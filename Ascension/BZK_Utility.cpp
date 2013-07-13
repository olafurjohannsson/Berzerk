
#include <Windows.h>

#include "BZK_Utility.h"

void BZK::ZeroMemBytes(char * A_dest, unsigned int A_size)
{
	::ZeroMemory((void *)A_dest, A_size);
}


void BZK::CopyMemBytes(char * A_dest, const char * A_src, unsigned int A_size)
{
	::CopyMemory((void *)A_dest, (void *)A_src, A_size);
}