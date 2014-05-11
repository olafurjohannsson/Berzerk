
#include "BRZ_Define.h"

#include <Windows.h>


void BRZ::TrimString(BRZSTRING & A_str)
{
	if (A_str.empty())
		return;

	const BRZSTRING		whitespace = L"\t\n ";

	BRZSTRING::size_type	trimStart = A_str.find_first_not_of(whitespace);
	BRZSTRING::size_type	trimEnd = A_str.find_last_not_of(whitespace);

	if (trimStart == BRZSTRING::npos)
	{
		A_str = L"";
		return;
	}

	A_str = A_str.substr(trimStart, (trimEnd + 1) - trimStart);
	return;
}


BRZSTRING BRZ::Widen(const std::string & A_src)
{
	// Allocate the buffer, and set the last character to the null terminator:
	wchar_t * buffer = new wchar_t[A_src.length() + 1];
	buffer[A_src.length()] = L'\0';

	// Copy the narrow string, and check for errors:
	size_t outSize = 0;
	unsigned int result = (unsigned int)::mbstowcs_s(&outSize, buffer, A_src.length() + 1, A_src.c_str(), A_src.length());
	BRZSTRING out = buffer;
	delete [] buffer;

	return out;
}


std::string BRZ::Narrow(const BRZSTRING & A_src)
{
	if (A_src.empty())
	{
		return "";
	}

	// Allocate the buffer, and set the last character to the null terminator:
	char * buffer = new char[A_src.length() + 1];
	buffer[A_src.length()] = '\0';

	// Copy the narrow string, and check for errors:
	size_t outSize;
	unsigned int result = (unsigned int)::wcstombs_s(&outSize, buffer, A_src.length() + 1, A_src.c_str(), A_src.length());
	std::string out = buffer;
	delete [] buffer;

	return out;
}


void BRZ::ZeroMemBytes(BRZBYTE * A_dest, unsigned int A_size)
{
	::ZeroMemory((void *)A_dest, A_size);
}


void BRZ::CopyMemBytes(BRZBYTE * A_dest, const BRZBYTE * A_src, unsigned int A_size)
{
	::CopyMemory((void *)A_dest, (void *)A_src, A_size);
}