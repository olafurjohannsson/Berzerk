
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Vec2.h")
#endif

#include "BRZ_Define.h"

#include "BRZ_Coord2.h"

namespace BRZ
{
	class Vec2
	{
	public:
		inline Vec2();
		inline Vec2(float x, float y);
		inline Vec2(const BRZ::Vec2 & ref);

		explicit inline Vec2(const BRZ::Coord2 & ref);

	public:
		inline bool operator ==(const BRZ::Vec2 & ref) const;
		inline bool operator !=(const BRZ::Vec2 & ref) const;
		inline bool operator !() const;

		inline const BRZ::Vec2 & operator=(const BRZ::Vec2 & arg);

		inline BRZ::Vec2 operator+(const BRZ::Vec2 & arg) const;
		inline BRZ::Vec2 operator-(const BRZ::Vec2 & arg) const;
		inline BRZ::Vec2 operator*(const BRZ::Vec2 & arg) const;
		inline BRZ::Vec2 operator*(float scalar) const;
		inline BRZ::Vec2 operator/(float scalar) const;
		inline BRZ::Vec2 operator/(const BRZ::Vec2 & arg) const;

		inline const BRZ::Vec2 & operator+=(const BRZ::Vec2 & arg);
		inline const BRZ::Vec2 & operator-=(const BRZ::Vec2 & arg);
		inline const BRZ::Vec2 & operator*=(float scalar);

		inline float Length() const;
		inline float SqrLength() const;
		inline BRZ::Vec2 Normalize() const;

	public:
		float		x;
		float		y;
	};
}


BRZ::Vec2 BRZ::Vec2::operator /(float A_scalar) const
{
	BRZ::Vec2 out(*this);

	out.x /= A_scalar;
	out.y /= A_scalar;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator /(const BRZ::Vec2 & A_ref) const
{
	BRZ::Vec2 out(*this);

	out.x /= A_ref.x;
	out.y /= A_ref.y;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator *(const BRZ::Vec2 & A_ref) const
{
	BRZ::Vec2 out(*this);

	out.x *= A_ref.x;
	out.y *= A_ref.y;

	return out;
}


bool BRZ::Vec2::operator ==(const BRZ::Vec2 & A_ref) const
{
	return (abs(x - A_ref.x) < BRZ::EPS && \
			abs(y - A_ref.y) < BRZ::EPS);
}


bool BRZ::Vec2::operator !=(const BRZ::Vec2 & A_ref) const
{
	return !(*this == A_ref);
}


bool BRZ::Vec2::operator !() const
{
	return (*this == BRZ::Vec2());
}


const BRZ::Vec2 & BRZ::Vec2::operator =(const BRZ::Vec2 & A_arg)
{
	x = A_arg.x;
	y = A_arg.y;

	return *this;
}


BRZ::Vec2 BRZ::Vec2::operator +(const BRZ::Vec2 & A_arg) const
{
	BRZ::Vec2 out(*this);
	out += A_arg;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator -(const BRZ::Vec2 & A_arg) const
{
	BRZ::Vec2 out(*this);
	out -= A_arg;

	return out;
}


BRZ::Vec2 BRZ::Vec2::operator *(float A_scal) const
{
	BRZ::Vec2 out(*this);
	out *= A_scal;

	return out;
}


const BRZ::Vec2 & BRZ::Vec2::operator +=(const BRZ::Vec2 & A_arg)
{
	x += A_arg.x;
	y += A_arg.y;

	return *this;
}


const BRZ::Vec2 & BRZ::Vec2::operator -=(const BRZ::Vec2 & A_arg)
{
	x -= A_arg.x;
	y -= A_arg.y;

	return *this;
}


const BRZ::Vec2 & BRZ::Vec2::operator *=(float A_scal)
{
	x *= A_scal;
	y *= A_scal;

	return *this;
}


float BRZ::Vec2::Length() const
{
	return sqrt(this->SqrLength());
}


float BRZ::Vec2::SqrLength() const
{
	return (pow(x, 2) + pow(y, 2));
}


BRZ::Vec2 BRZ::Vec2::Normalize() const
{
	BRZ::Vec2 out;
	if (!(*this))
		return out;

	float fac = this->Length();
	out.x = x / fac;
	out.y = y / fac;
	return out;
}


BRZ::Vec2::Vec2() :
	x(0.0f),
	y(0.0f)
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


BRZ::Vec2::Vec2(const BRZ::Coord2 & A_ref) :
	x(A_ref.x),
	y(A_ref.y)
{
}
