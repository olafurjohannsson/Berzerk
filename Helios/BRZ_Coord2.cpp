
#include "BRZ_Coord2.h"

signed int BRZ::Coord2::BAD_INDEX = 0xBAD;


BRZ::Coord2 BRZ::Coord2::operator+(const BRZ::Coord2 & A_ref) const
{
	BRZ::Coord2 out(*this);

	out.x += A_ref.x;
	out.y += A_ref.y;

	return out;
}


signed int & BRZ::Coord2::operator[](unsigned int A_idx)
{
	switch(A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	default:		return BAD_INDEX;
	}
}


const signed int & BRZ::Coord2::operator[](unsigned int A_idx) const
{
	switch(A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	default:		return BAD_INDEX;
	}
}


bool BRZ::Coord2::IsNull() const
{
	if (x == 0 && y == 0)
		return true;
	else
		return false;
}


bool BRZ::Coord2::IsMixed() const
{
	if ((x < 0 && y >= 0) || (x >= 0 && y < 0))
		return true;
	else
		return false;
}


bool BRZ::Coord2::IsNegative() const
{
	if (x < 0 && y < 0)
		return true;
	else
		return false;
}


bool BRZ::Coord2::IsPositive() const
{
	if (x >= 0 && y >= 0)
		return true;
	else
		return false;
}


BRZ::Coord2 BRZ::Coord2::Abs() const
{
	BRZ::Coord2 out(*this);

	if (out.x < 0)
		out.x *= -1;

	if (out.y < 0)
		out.y *= -1;

	return out;
}


BRZ::Coord2::Coord2() :
	x(0),
	y(0)
{
}


BRZ::Coord2::Coord2(signed int A_x, signed int A_y) :
	x(A_x),
	y(A_y)
{
}


BRZ::Coord2::Coord2(const BRZ::Coord2 & A_ref) :
	x(A_ref.x),
	y(A_ref.y)
{
}


BRZ::Coord2::~Coord2()
{
	x = 0xBAE;
	y = 0xBAF;
}

