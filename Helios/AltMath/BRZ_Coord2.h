
#pragma once

#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Coord2.h")
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Coord2
	{
	public:
		Coord2() : x(0), y(0)	{}
		Coord2(signed int A_x, signed int A_y) : x(A_x), y(A_y)	{}

		inline BRZ::Coord2 operator*(signed int factor) const;
		inline BRZ::Coord2 operator+(const BRZ::Coord2 & ref) const;
		inline BRZ::Coord2 operator-(const BRZ::Coord2 & ref) const;
		inline bool operator==(const BRZ::Coord2 & ref) const;
		inline bool operator!=(const BRZ::Coord2 & ref) const;
		inline bool operator!() const;

		signed int x;
		signed int y;
	protected:
	private:
	};
}



bool BRZ::Coord2::operator !() const
{
	return ((x == 0) && (y == 0));
}


BRZ::Coord2 BRZ::Coord2::operator *(signed int A_fac) const
{
	BRZ::Coord2 out(*this);

	out.x *= A_fac;
	out.y *= A_fac;

	return out;
}


BRZ::Coord2 BRZ::Coord2::operator +(const BRZ::Coord2 & A_ref) const
{
	BRZ::Coord2 out(*this);
	out.x += A_ref.x;
	out.y += A_ref.y;
	return out;
}


BRZ::Coord2 BRZ::Coord2::operator -(const BRZ::Coord2 & A_ref) const
{
	BRZ::Coord2 out(*this);
	out.x -= A_ref.x;
	out.y -= A_ref.y;
	return out;
}


bool BRZ::Coord2::operator ==(const BRZ::Coord2 & A_ref) const
{
	if (x == A_ref.x && y == A_ref.y)
		return true;
	else
		return false;
}


bool BRZ::Coord2::operator !=(const BRZ::Coord2 & A_ref) const
{
	return !(*this == A_ref);
}
