
#pragma once

#ifdef BZK_HEADERTRACE
#pragma message "Header Trace: BZK_Utility.h"
#endif

namespace BZK
{
	void ZeroMemBytes(char * destination, unsigned int size);
	void CopyMemBytes(char * destination, const char * source, unsigned int size);

	template <class T_type>
	void ZeroMem(T_type * destination, unsigned int unitCount = 1);

	template <class T_type>
	void CopyMem(T_type * destination, const T_type * source, unsigned int unitCount = 1);
}

template <class T_type>
void BZK::ZeroMem(T_type * A_dest, unsigned int A_count)
{
	if (A_count == 0)
		return;

	return BZK::ZeroMemBytes((char *)A_dest, A_count * sizeof(T_type));
}


template <class T_type>
void BZK::CopyMem(T_type * A_dest, const T_type * A_src, unsigned int A_count)
{
	if (A_count == 0)
		return;

	return BZK::CopyMemBytes((char *)A_dest, (char *)A_src, A_count * sizeof(T_type));
}