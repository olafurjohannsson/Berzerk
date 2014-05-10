
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Define.h" )
#endif


#	pragma warning(disable : 4786)	// Truncated identifier name due to length warning
#	pragma warning(disable : 4127)  // Constant comparison warning
#	pragma warning(disable : 4244)  // ASCFLOAT <-> ASCDOUBLE conversion, possible data loss
#	pragma warning(disable : 4512)	// Lack of assignment operator warning
#	pragma warning(disable : 4511)	// Lack of copy constructor warning

#	pragma warning(disable : 4100)	// Unreferenced paramater warning -> error
#	pragma warning(disable : 4702)	// Unreachable code warning


#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>

typedef char				BRZBYTE;
typedef wchar_t				BRZCHAR;
typedef std::wstring		BRZSTRING;
typedef unsigned int		BRZRESULT;
typedef DirectX::XMMATRIX	BRZMATRIX;
typedef std::wistringstream	BRZISTREAM;
typedef std::wostringstream	BRZOSTREAM;

#define BRZ_SUCCESS		0
#define BRZ_FAILURE		1

// Certain mathematical constants and functions:
namespace BRZ
{
	const float EPS = 0.0000001f;
	const float PI = DirectX::XM_PI;
	const float HALF_PI = BRZ::PI / 2.0f;
	const float TWO_PI = BRZ::PI * 2.0f;

	inline float ClampRotation(float inRotation);
	inline float DegreeToRad(float inDegree);
	inline float RadToDegree(float inRadian);
}


// Forward declarations for general core classes:
namespace BRZ
{
	class Config;
	class DiskStream;
	class Time;
	class Window;
}


// Forward declaration for graphics module classes:
namespace BRZ
{
	class Colour;
	class Display;
	class LineTemplate;
	class LineElement;

	class LineObject;
	

	class RawElement;
	class RawGeometry;
}


// Forward declaration for math classes:
namespace BRZ
{
	class Coord2;
	class Vec2;
}



namespace BRZ
{
	// String utility functions:
	BRZSTRING		Widen(const std::string & str);
	std::string		Narrow(const BRZSTRING & str);
	void			TrimString(BRZSTRING & string);

	// General memory utility functions:
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


float BRZ::ClampRotation(float A_in)
{
	while (A_in > BRZ::TWO_PI)
		A_in -= BRZ::TWO_PI;

	while (A_in < 0.0f)
		A_in += BRZ::TWO_PI;

	return A_in;
}


float BRZ::DegreeToRad(float A_in)
{
	return ((A_in / 360.0f) * BRZ::TWO_PI);
}


float BRZ::RadToDegree(float A_in)
{
	return ((A_in / BRZ::TWO_PI) * 360.0f);
}