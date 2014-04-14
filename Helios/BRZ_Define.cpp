
#include "BRZ_Define.h"

#include <Windows.h>

void BRZ::ZeroMemBytes(BRZBYTE * A_dest, unsigned int A_size)
{
	::ZeroMemory((void *)A_dest, A_size);
}


void BRZ::CopyMemBytes(BRZBYTE * A_dest, const BRZBYTE * A_src, unsigned int A_size)
{
	::CopyMemory((void *)A_dest, (void *)A_src, A_size);
}