
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Coord3.h")
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Coord3
	{
	public:
		Coord3() : x(0), y(0), z(0)												{	}
		Coord3(signed int x, signed int y, signed int z) : x(x), y(y), z(z)		{	}
	public:
		signed int		x;
		signed int		y;
		signed int		z;
	public:
		inline signed int & operator[] (unsigned int index);
		inline const signed int & operator[] (unsigned int index) const;
	protected:
	private:
	};
}


signed int & BRZ::Coord3::operator [] (unsigned int A_idx)
{

	switch (A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	case 2:			return z;
	default:		return x;
	}
}


const signed int & BRZ::Coord3::operator [] (unsigned int A_idx) const
{
	switch (A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	case 2:			return z;
	default:		return x;
	}
}
