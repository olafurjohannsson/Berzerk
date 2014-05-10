
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_DiskStream" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class DiskStream
	{
	public:
		DiskStream();
		DiskStream(const DiskStream & ref);
		~DiskStream();

	public:
		BRZRESULT		OpenRead(const BRZSTRING & file);
		BRZRESULT		OpenWrite(const BRZSTRING & file);
		BRZRESULT		Close();

		unsigned int	Size() const;

	public:
		// File reading functions:
		BRZRESULT Unpack(BRZSTRING & output);
		BRZRESULT Unpack(signed int & output);
		BRZRESULT Unpack(float & output);
		BRZRESULT Unpack(bool & output);
		BRZRESULT Unpack(BRZBYTE * outArray, unsigned int size);
		template <class T_type>
		BRZRESULT Unpack(T_type & output);

	public:
		// File writing functions:
		BRZRESULT Pack(const BRZSTRING & input);
		BRZRESULT Pack(const signed int & input);
		BRZRESULT Pack(const float & input);
		BRZRESULT Pack(const bool & input);
		BRZRESULT Pack(const BRZBYTE * inArray, unsigned int size);
		template <class T_type>
		BRZRESULT Pack(const T_type & output);

	private:
		std::fstream	file;
		unsigned int	size;
		bool			open;
		bool			read;
		bool			write;
		bool			error;
	};
}

template <class T_type>
BRZRESULT BRZ::DiskStream::Unpack(T_type & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	A_dest.Unpack(*this);

	return BRZ_SUCCESS;
}


template <class T_type>
BRZRESULT BRZ::DiskStream::Pack(const T_type & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	unsigned int deltaSize = A_dest.Pack(*this);

	if (deltaSize == 0xFFFFFFFF)
		return BRZ_FAILURE;

	size += deltaSize;
	return BRZ_SUCCESS;
}