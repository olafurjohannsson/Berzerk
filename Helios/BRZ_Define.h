
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message "Header Trace: BRZ_Define.h"
#endif


#	pragma warning(disable : 4786)	// Truncated identifier name due to length warning
#	pragma warning(disable : 4127)  // Constant comparison warning
#	pragma warning(disable : 4244)  // ASCFLOAT <-> ASCDOUBLE conversion, possible data loss
#	pragma warning(disable : 4512)	// Lack of assignment operator warning
#	pragma warning(disable : 4511)	// Lack of copy constructor warning

#	pragma warning(disable : 4100)	// Unreferenced paramater warning -> error
#	pragma warning(disable : 4702)	// Unreachable code warning


#include <string>
// #include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

typedef char				BRZBYTE;
typedef wchar_t				BRZCHAR;
typedef std::wstring		BRZSTRING;
typedef unsigned int		BRZRESULT;
typedef DirectX::XMMATRIX	BRZMATRIX;

#define BRZ_SUCCESS		0
#define BRZ_FAILURE		1

namespace BRZ
{
	const float EPS = 0.000000001f;
	const float PI = DirectX::XM_PI;
	const float HALF_PI = BRZ::PI / 2.0f;
	const float TWO_PI = BRZ::PI * 2.0f;
}



namespace BRZ
{
	void ZeroMemBytes(BRZBYTE * destination, unsigned int size);
	void CopyMemBytes(BRZBYTE * destination, const BRZBYTE * source, unsigned int size);

	template <class T_type>
	void ZeroMem(T_type * destination, unsigned int unitCount = 1);

	template <class T_type>
	void CopyMem(T_type * destination, const T_type * source, unsigned int unitCount = 1);
}

template <class T_type>
void BRZ::ZeroMem(T_type * A_dest, unsigned int A_count)
{
	if (A_count == 0)
		return;

	return BRZ::ZeroMemBytes((BRZBYTE *)A_dest, A_count * sizeof(T_type));
}


template <class T_type>
void BRZ::CopyMem(T_type * A_dest, const T_type * A_src, unsigned int A_count)
{
	if (A_count == 0)
		return;

	return BRZ::CopyMemBytes((BRZBYTE *)A_dest, (BRZBYTE *)A_src, A_count * sizeof(T_type));
}