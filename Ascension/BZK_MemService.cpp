

#include <fstream>

#include "BZK_MemService.h"
#include "BZK_Utility.h"

// GLOBAL single instance of MemService class:
BZK::MemService * memSvc = NULL;


BZK::MemService::MemService(unsigned int A_tableSize) :
	table(NULL),
	tableSize(A_tableSize),
	nextTicket(0),
	remaining(A_tableSize),
	lastVerified(NULL)
{
	table = new BZK::MemTicket[A_tableSize];
}


BZK::MemService::~MemService()
{
	this->LogMemoryUsage("MemLeaks.log");
	delete table;
}


char * BZK::MemService::Alloc(unsigned int A_size)
{
	if (remaining == 0)
		return NULL;

	char * alloc = (char *)malloc(A_size);
	BZK::ZeroMem(alloc, A_size);

	table[nextTicket].type = BZK::OBJ_ALLOC;
	table[nextTicket].count = 1;
	table[nextTicket].unitSize = A_size;
	table[nextTicket].data = alloc;

	nextTicket++;
	remaining--;

	return alloc;
}


char * BZK::MemService::ArrayAlloc(unsigned int A_size, unsigned int A_count)
{
	if (remaining == 0)
		return NULL;

	char * alloc = (char *)malloc(A_size * A_count);
	BZK::ZeroMem(alloc, A_size * A_count);

	table[nextTicket].type = BZK::ARRAY_ALLOC;
	table[nextTicket].count = A_count;
	table[nextTicket].unitSize = A_size;
	table[nextTicket].data = alloc;

	nextTicket++;
	remaining--;

	return alloc;
}


char * BZK::MemService::RawAlloc(unsigned int A_size)
{
	if (remaining == 0)
		return NULL;

	char * alloc = (char *)malloc(A_size);
	BZK::ZeroMem(alloc, A_size);

	table[nextTicket].type = BZK::RAW_ALLOC;
	table[nextTicket].count = 1;
	table[nextTicket].unitSize = A_size;
	table[nextTicket].data = alloc;

	nextTicket++;
	remaining--;

	return alloc;
}


unsigned int BZK::MemService::Free(char * A_data)
{
	// Verify that the pointer being freed is the last pointer to have had it's legitimacy
	//	determined;

	if (lastVerified == NULL)
		return 1;
	else if (lastVerified->data != A_data)
		return 2;

	// Determine where in the table the ticket lies with pointer math; Then copy every entry 
	//	further down one space up.
	unsigned int offset = (lastVerified - table);
	for (unsigned int i = offset; i < (nextTicket - 1); ++i)
	{
		BZK::CopyMem(&table[i], &table[i + 1]);
	}

	nextTicket--;
	remaining++;
	lastVerified = NULL;

	free((void *)A_data);

	return 0;
}


void BZK::MemService::LogMemoryUsage(const std::string & A_log) const
{
	std::ofstream out(A_log.c_str());

	unsigned int total = 0;
	out << "MEMORY USAGE LOG [" << nextTicket << " Allocations]:" << std::endl << std::endl;

	for (unsigned int i = 0; i < nextTicket; ++i)
	{
		out << "Memory Allocation of type: <";
		if (table[i].type == BZK::BAD_ALLOC)
		{
			out << "BAD ALLOC> -- Size: " << table[i].unitSize << " bytes." << std::endl;
			total += table[i].unitSize;
			continue;
		}
		else if (table[i].type == BZK::OBJ_ALLOC)
		{
			out << "Object> -- Size: " << table[i].unitSize << " bytes." << std::endl;
			total += table[i].unitSize;
			continue;
		}
		else if (table[i].type == BZK::ARRAY_ALLOC)
		{
			out << "Array> -- Spread: " << table[i].count << " objects -- Object Size: " << table[i].unitSize;
			out <<" bytes; Total Size: " << table[i].count * table[i].unitSize << " bytes." << std::endl;
			total += (table[i].unitSize * table[i].count);
			continue;
		}
		else if (table[i].type == BZK::RAW_ALLOC)
		{
			out << "Raw> -- Size: " << table[i].unitSize << " bytes." << std::endl;
			total += table[i].unitSize;
			continue;
		}
	}

	out << std::endl << "Total Memory Currently Allocated through Tracker: " << total << " bytes." << std::endl;

	out.close();
}


const BZK::MemTicket * BZK::MemService::Info() const
{
	return lastVerified;
}


bool BZK::MemService::IsLegit(const char * A_data)
{
	for (unsigned int i = 0; i < nextTicket; ++i)
	{
		if (table[i].data == A_data)
		{
			lastVerified = &table[i];
			return true;
		}
	}

	return false;
}


void BZK::InitMemService()
{
	memSvc = new BZK::MemService(64);
}


void BZK::EndMemService()
{
	delete memSvc;
	memSvc = NULL;
}


BZK::MemService * BZK::GetMemService()
{
	return memSvc;
}


char * BZK::RawAlloc(unsigned int A_size)
{
	BZK::MemService * svc = BZK::GetMemService();
	if (svc == NULL)
		return NULL;

	return svc->RawAlloc(A_size);
}