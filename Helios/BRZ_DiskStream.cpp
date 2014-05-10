
#include "BRZ_DiskStream.h"


BRZRESULT BRZ::DiskStream::Pack(const BRZBYTE * A_src, unsigned int A_size)
{
	if (!open || !write || error)
		return BRZ_FAILURE;

	file.write(A_src, A_size);
	size += A_size;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Pack(const bool & A_src)
{
	if (!open || !write || error)
		return BRZ_FAILURE;

	file.write((const BRZBYTE *)&A_src, sizeof(A_src));
	size += sizeof(A_src);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Pack(const float & A_src)
{
	if (!open || !write || error)
		return BRZ_FAILURE;

	file.write((const BRZBYTE *)&A_src, sizeof(A_src));
	size += sizeof(A_src);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Pack(const signed int & A_src)
{
	if (!open || !write || error)
		return BRZ_FAILURE;

	file.write((const BRZBYTE *)&A_src, sizeof(A_src));
	size += sizeof(A_src);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Pack(const BRZSTRING & A_src)
{
	if (!open || !write || error)
		return BRZ_FAILURE;

	// Write the number of string characters:
	unsigned int strSize = A_src.length();
	file.write((const BRZBYTE *)&strSize, sizeof(strSize));
	size += sizeof(strSize);

	if (strSize == 0)
		return BRZ_SUCCESS;

	// Write the wide characters themselves, sans null terminator:
	file.write((const BRZBYTE *)A_src.c_str(), strSize * sizeof(BRZCHAR));
	size += (strSize * sizeof(BRZCHAR));

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Unpack(BRZBYTE * A_dest, unsigned int A_size)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	if (A_dest == NULL)
		return BRZ_FAILURE;

	if ((size - file.tellg()) < A_size)
	{
		error = true;
		return BRZ_FAILURE;
	}

	file.read(A_dest, A_size);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Unpack(bool & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	if ((size - file.tellg()) < sizeof(A_dest))
	{
		error = true;
		return BRZ_FAILURE;
	}

	file.read((BRZBYTE *)&A_dest, sizeof(A_dest));

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Unpack(float & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	if ((size - file.tellg()) < sizeof(A_dest))
	{
		error = true;
		return BRZ_FAILURE;
	}

	file.read((BRZBYTE *)&A_dest, sizeof(A_dest));

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Unpack(signed int & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	if ((size - file.tellg()) < sizeof(A_dest))
	{
		error = true;
		return BRZ_FAILURE;
	}

	file.read((BRZBYTE *)&A_dest, sizeof(A_dest));

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::Unpack(BRZSTRING & A_dest)
{
	if (!open || !read || error)
		return BRZ_FAILURE;

	if (file.eof())
		return BRZ_FAILURE;

	// Verify the string character count exists in the file:
	if ((size - file.tellg()) < sizeof(unsigned int))
	{
		error = true;
		return BRZ_FAILURE;
	}

	// Read character count of string:
	unsigned int	chars;
	file.read((BRZBYTE *)&chars, sizeof(chars));
	if (chars == 0)
	{
		A_dest == L"";
		return BRZ_SUCCESS;
	}

	// Verify the string itself exists in the file:
	if ((size - file.tellg()) < (chars * sizeof(BRZCHAR)))
	{
		error = true;
		return BRZ_FAILURE;
	}

	// Allocate buffer, read in wide string:
	BRZCHAR *		string = new BRZCHAR[(chars + 1)];
	BRZ::ZeroMem(string, (chars + 1));
	file.read((BRZBYTE *)string, chars * sizeof(BRZCHAR));
	A_dest = string;

	return BRZ_SUCCESS;
}


unsigned int BRZ::DiskStream::Size() const
{
	return size;
}


BRZRESULT BRZ::DiskStream::Close()
{
	if (!open)
		return BRZ_FAILURE;

	file.close();
	size = 0;
	open = false;
	read = false;
	write = false;
	error = false;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::OpenRead(const BRZSTRING & A_file)
{
	if (open)
		return BRZ_FAILURE;

	file.open(BRZ::Narrow(A_file).c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
		return BRZ_FAILURE;
	else
	{
		open = true;
		read = true;
	}

	// Determine the file size:
	std::fstream::streampos		begin = 0;
	file.seekg(std::ios::end, 0);
	std::fstream::streampos		end = file.tellg();
	file.seekg(std::ios::beg, 0);
	size = end - begin;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::DiskStream::OpenWrite(const BRZSTRING & A_file)
{
	if (open)
		return BRZ_FAILURE;

	file.open(BRZ::Narrow(A_file).c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open())
		return BRZ_FAILURE;
	else
	{
		open = true;
		write = true;
	}

	size = 0;

	return BRZ_SUCCESS;
}


BRZ::DiskStream::DiskStream() :
	size(0),
	open(false),
	read(false),
	write(false),
	error(false)
{
}


BRZ::DiskStream::~DiskStream()
{
	if (open)
		this->Close();
}