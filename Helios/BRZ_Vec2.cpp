
#include "BRZ_Vec2.h"


float BRZ::Vec2::BAD_INDEX = 1349.0f;


float & BRZ::Vec2::operator[](unsigned int A_idx)
{
	switch (A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	default:		return BAD_INDEX;
	}
}


const float & BRZ::Vec2::operator[](unsigned int A_idx) const
{
	switch (A_idx)
	{
	case 0:			return x;
	case 1:			return y;
	default:		return BAD_INDEX;
	}
}


BRZ::Vec2::Vec2() :
	x(0),
	y(0)
{
}


BRZ::Vec2::Vec2(float A_x, float A_y) :
	x(A_x),
	y(A_y)
{
}


BRZ::Vec2::Vec2(const BRZ::Vec2 & A_ref) :
	x(A_ref.x),
	y(A_ref.y)
{
}


BRZ::Vec2::~Vec2()
{
	x = 1350.0f;
	y = 1351.0f;
}