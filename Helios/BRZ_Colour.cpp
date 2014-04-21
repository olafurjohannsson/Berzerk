
#include "BRZ_Colour.h"

float BRZ::Colour::BAD_INDEX	= -1.0f;


float & BRZ::Colour::operator[](unsigned int A_idx)
{
	switch (A_idx)
	{
	case 0:			return r;
	case 1:			return g;
	case 2:			return b;
	case 3:			return a;
	default:		return BAD_INDEX;
	}
}


const float & BRZ::Colour::operator[](unsigned int A_idx) const
{
	switch (A_idx)
	{
	case 0:			return r;
	case 1:			return g;
	case 2:			return b;
	case 3:			return a;
	default:		return BAD_INDEX;
	}
}


BRZ::Colour::Colour() :
	r(0),
	g(0),
	b(0),
	a(0)
{
}


BRZ::Colour::Colour(float A_r, float A_g, float A_b, float A_a) :
	r(A_r),
	g(A_g),
	b(A_b),
	a(A_a)
{
}


BRZ::Colour::Colour(const BRZ::Colour & A_ref) :
	r(A_ref.r),
	g(A_ref.g),
	b(A_ref.b),
	a(A_ref.a)
{
}


BRZ::Colour::~Colour()
{
	r = -2;
	g = -3;
	b = -4;
	a = -5;
}