
#include "BRZ_Vec2.h"

#include "BRZ_DiskStream.h"


float BRZ::Vec2::BAD_INDEX = 1349.0f;


//	Note: Function rotates this specific vector by THETA radians, and returns
//			the rotated vector.  Note that the method does NOT change this vector.
BRZ::Vec2 BRZ::Vec2::Rotate(float A_theta) const
{
	A_theta = BRZ::ClampRotation(A_theta);

	BRZ::Vec2	out(0, 0);
	BRZ::Vec2	xContribution(cos(A_theta), sin(A_theta));
	xContribution *= this->x;

	BRZ::Vec2	yContribution(cos(BRZ::ClampRotation(A_theta + BRZ::HALF_PI)), sin(BRZ::ClampRotation(A_theta + BRZ::HALF_PI)));
	yContribution *= this->y;

	out = xContribution + yContribution;

	return out;
}


float BRZ::Vec2::SqrLength() const
{
	return ((x * x) + (y * y));
}


float BRZ::Vec2::Length() const
{
	return sqrt(this->SqrLength());
}


BRZ::Vec2 BRZ::Vec2::Normalize() const
{
	BRZ::Vec2	out(*this);
	float		len(this->Length());

	out.x /= len;
	out.y /= len;

	return out;
}


const BRZ::Vec2 & BRZ::Vec2::operator*=(const float & A_scalar)
{
	this->x *= A_scalar;
	this->y *= A_scalar;

	return *this;
}


const BRZ::Vec2 & BRZ::Vec2::operator-=(const BRZ::Vec2 & A_sub)
{
	this->x -= A_sub.x;
	this->y -= A_sub.y;

	return *this;
}


const BRZ::Vec2 & BRZ::Vec2::operator+=(const BRZ::Vec2 & A_add)
{
	this->x += A_add.x;
	this->y += A_add.y;

	return *this;
}


BRZ::Vec2 BRZ::Vec2::operator+(const BRZ::Vec2 & A_add) const
{
	BRZ::Vec2 out(*this);

	out.x += A_add.x;
	out.y += A_add.y;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator-(const BRZ::Vec2 & A_sub) const
{
	BRZ::Vec2 out(*this);

	out.x -= A_sub.x;
	out.y -= A_sub.y;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator*(const float & A_scalar) const
{
	BRZ::Vec2 out(*this);

	out.x *= A_scalar;
	out.y *= A_scalar;

	return out;
}


BRZRESULT BRZ::Vec2::Unpack(BRZ::DiskStream & A_in)
{
	if (A_in.Unpack(x) == BRZ_FAILURE ||
		A_in.Unpack(y) == BRZ_FAILURE)
	{
		return BRZ_FAILURE;
	}

	return BRZ_SUCCESS;
}


unsigned int BRZ::Vec2::Pack(BRZ::DiskStream & A_out) const
{
	if (A_out.Pack(x) == BRZ_FAILURE ||
		A_out.Pack(y) == BRZ_FAILURE)
	{
		return 0xFFFFFFFF;
	}

	return (2 * sizeof(float));
}


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