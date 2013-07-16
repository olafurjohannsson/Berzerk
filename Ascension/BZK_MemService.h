#pragma once

#ifdef BZK_HEADERTRACE
#pragma message "Header Trace: BZK_MemService.h"
#endif

#include <string>

namespace BZK
{
	class MemService;

	// Global functions to initialize single instance of memory service, and to acquire
	// a pointer to said instance where necessary.  Declared at global scope so template function
	// definitions below can use them.
	void				InitMemService();
	void				EndMemService();
	BZK::MemService *	GetMemService();


	// Global non-type-specific allocation and deletion functions for general usage:
	template <class T_obj>
	T_obj * Alloc();
	
	template <class T_obj, class T_arg1>
	T_obj * Alloc(const T_arg1 & arg1);

	template <class T_obj, class T_arg1, class T_arg2>
	T_obj * Alloc(const T_arg1 & arg1, const T_arg2 & arg2);

	template <class T_obj>
	T_obj * ArrayAlloc(unsigned int count);

	char * RawAlloc(unsigned int size);

	template <class T_obj>
	unsigned int Free(T_obj * data);


	// Possible types of allocations:
	enum MemType
	{
		BAD_ALLOC		= 0,
		OBJ_ALLOC,
		ARRAY_ALLOC,
		RAW_ALLOC
	};

	struct MemTicket
	{
		MemTicket() : type(BAD_ALLOC), count(0), unitSize(0), data(NULL)		{	}

		BZK::MemType		type;
		unsigned int		count;
		unsigned int		unitSize;
		char *				data;
	};


	class MemService
	{
	public:
		MemService(unsigned int tableSize);
		~MemService();


	public:
		char *			Alloc(unsigned int unitSize);
		char *			ArrayAlloc(unsigned int unitSize, unsigned int unitCount);
		char *			RawAlloc(unsigned int chunkSize);

		// In cases of successful memory freeing, function returns 0.
		unsigned int	Free(char * data);

		void			LogMemoryUsage(const std::string & logName) const;

		// Non-Class specific interface for validating if a pointer was allocated through
		//	MemService class.
		template <class T_obj>	
			bool		IsLegit(const T_obj * data);

		// Acquires the ticket information from the LAST pointer verified through IsLegit,
		//	required to determine whether an allocation being freed is an array, and subsequently
		//	how many units it contains, to ensure the destructor is invoked for every object.
		const BZK::MemTicket *	Info() const;
	protected:
	private:
		// Determine if a ticket for the allocation exists, e.g. the memory needs to be freed
		//	through MemService interfaces.
		bool			IsLegit(const char * data);

	private:
		BZK::MemTicket *		table;

		unsigned int			tableSize;
		unsigned int			nextTicket;
		unsigned int			remaining;

		const BZK::MemTicket *	lastVerified;
	};
}


template <class T_obj>
bool BZK::MemService::IsLegit(const T_obj * A_data)
{
	return this->IsLegit((const char *)A_data);
}


template <class T_obj>
T_obj * BZK::Alloc()
{
	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
	{
		// Memory services haven't been initialized?
		return NULL;
	}
	else
	{
		T_obj * ptr = (T_obj *)svc->Alloc(sizeof(T_obj));
		new(ptr) T_obj();

		return ptr;
	}
}


template <class T_obj, class T_arg1>
T_obj * BZK::Alloc(const T_arg1 & A_arg1)
{
	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
		return NULL;
	else
	{
		T_obj * ptr = (T_obj *)svc->Alloc(sizeof(T_obj));
		new(ptr) T_obj(A_arg1);

		return ptr;
	}
}


template <class T_obj, class T_arg1, class T_arg2>
T_obj * BZK::Alloc(const T_arg1 & A_arg1, const T_arg2 & A_arg2)
{
	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
		return NULL;
	else
	{
		T_obj * ptr = (T_obj *)svc->Alloc(sizeof(T_obj));
		new(ptr) T_obj(A_arg1, A_arg2);

		return ptr;
	}
}


template <class T_obj>
T_obj * BZK::ArrayAlloc(unsigned int A_count)
{
	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
		return NULL;
	else
	{
		T_obj * ptr = (T_obj *)svc->ArrayAlloc(sizeof(T_obj), A_count);
		T_obj * iter = ptr;
		for (unsigned int i = 0; i < A_count; ++i)
		{
			new(iter) T_obj();
			iter++;
		}

		return ptr;
	}
}


//	NOTE:	If BZK::Free does not return 0, an error occurred.
template <class T_obj>
unsigned int BZK::Free(T_obj * A_data)
{
	if (A_data == NULL)
		return 0;

	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
		return 1;
	
	// Verify pointer legitimacy.
	if (!svc->IsLegit(A_data))
		return 2;

	// Determine the type of allocation, and invoke destructors for all the objects
	//	being freed.
	const BZK::MemTicket * info = svc->Info();
	if (info->type == BZK::BAD_ALLOC)
	{
		return 3;
	}
	else if (info->type == BZK::OBJ_ALLOC)
	{
		A_data->~T_obj();
	}
	else if (info->type == BZK::ARRAY_ALLOC)
	{
		for (unsigned int i = 0; i < info->count; ++i)
		{
			A_data[i].~T_obj();
		}
	}
	else if (info->type == BZK::RAW_ALLOC)
	{
		// Do nothing.
	}

	// Once all relevant destructors have been called, notify the memory service to
	//	remove the relevant ticket and free the memory back to the operating system.
	return svc->Free((char *)A_data);
}