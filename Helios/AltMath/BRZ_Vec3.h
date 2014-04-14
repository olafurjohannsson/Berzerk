
#pragma once

#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Vec3.hpp")
#endif

#include "BRZ_Define.h"

#include "BRZ_Coord2.h"
#include "BRZ_Coord3.h"
#include "BRZ_Vec2.h"

#include <math.h>
// #include <d3dx9.h>

namespace BRZ
{
	class Vec3
	{
	public:
		Vec3() : x(0), y(0), z(0)												{	}
		Vec3(float x, float y, float z) : x(x), y(y), z(z)				{	}
		explicit Vec3(const BRZ::Vec2 & ref) : x(ref.x), y(ref.y), z(0.0f)		{	}
		explicit Vec3(const BRZ::Coord2 & ref) : x(ref.x), y(ref.y), z(0.0f)	{	}
		explicit Vec3(const BRZ::Coord3 & ref) : x(ref.x), y(ref.y), z(ref.z)	{	}
	public:

		/*
		inline operator ::D3DXVECTOR3 () const
		{	::D3DXVECTOR3 out;

			out.x = x;
			out.y = y;
			out.z = z;

			return out;					} */

		inline bool operator ==(const BRZ::Vec3 & ref) const;
		inline bool operator !=(const BRZ::Vec3 & ref) const;
		inline bool operator !() const;

		inline const BRZ::Vec3 & operator=(const BRZ::Vec3 & arg);
//		inline const BRZ::Vec3 & operator=(const ::D3DXVECTOR3 & arg);

		inline BRZ::Vec3 operator+(const BRZ::Vec3 & arg) const;
		inline BRZ::Vec3 operator-(const BRZ::Vec3 & arg) const;
		inline BRZ::Vec3 operator*(float scalar) const;

		inline const BRZ::Vec3 & operator+=(const BRZ::Vec3 & arg);
		inline const BRZ::Vec3 & operator-=(const BRZ::Vec3 & arg);
		inline const BRZ::Vec3 & operator*=(float scalar);

		inline BRZ::Vec3 Cross(const BRZ::Vec3 & arg) const;
		inline float Dot(const BRZ::Vec3 & arg) const;
		inline float Length() const;
		inline float SqrLength() const;
		inline BRZ::Vec3 Normalize() const;
	public:
		float	x;
		float	y;
		float	z;
	protected:
	private:
	};
}

/*
const BRZ::Vec3 & BRZ::Vec3::operator =(const ::D3DXVECTOR3 & A_ref)
{
	x = A_ref.x;
	y = A_ref.y;
	z = A_ref.z;

	return *this;
}
*/


const BRZ::Vec3 & BRZ::Vec3::operator =(const BRZ::Vec3 & A_ref)
{
	x = A_ref.x;
	y = A_ref.y;
	z = A_ref.z;

	return *this;
}


bool BRZ::Vec3::operator ==(const BRZ::Vec3 & A_ref) const
{
	return (x == A_ref.x) && (y == A_ref.y) && (z == A_ref.z);
}


bool BRZ::Vec3::operator !=(const BRZ::Vec3 & A_ref) const
{
	return !(*this == A_ref);
}


bool BRZ::Vec3::operator !() const
{
	return (*this == BRZ::Vec3());
}


BRZ::Vec3 BRZ::Vec3::operator+(const BRZ::Vec3 & A_ref) const
{
	BRZ::Vec3 out(x, y, z);
	out.x += A_ref.x;
	out.y += A_ref.y;
	out.z += A_ref.z;
	return out;
}


BRZ::Vec3 BRZ::Vec3::operator-(const BRZ::Vec3 & A_ref) const
{
	BRZ::Vec3 out(x, y, z);
	out.x -= A_ref.x;
	out.y -= A_ref.y;
	out.z -= A_ref.z;
	return out;
}


BRZ::Vec3 BRZ::Vec3::operator*(float A_scalar) const
{
	BRZ::Vec3 out(x, y, z);
	out.x *= A_scalar;
	out.y *= A_scalar;
	out.z *= A_scalar;
	return out;
}


const BRZ::Vec3 & BRZ::Vec3::operator+=(const BRZ::Vec3 & A_ref)
{
	x += A_ref.x;
	y += A_ref.y;
	z += A_ref.z;

	return *this;
}


const BRZ::Vec3 & BRZ::Vec3::operator-=(const BRZ::Vec3 & A_ref)
{
	x -= A_ref.x;
	y -= A_ref.y;
	z -= A_ref.z;

	return *this;
}


const BRZ::Vec3 & BRZ::Vec3::operator*=(float A_scalar)
{
	x *= A_scalar;
	y *= A_scalar;
	z *= A_scalar;

	return *this;
}


BRZ::Vec3 BRZ::Vec3::Cross(const BRZ::Vec3 & A_ref) const
{
	BRZ::Vec3 out;
	out.x = (this->y * A_ref.z) - (this->z * A_ref.y);
	out.y = (this->z * A_ref.x) - (this->x * A_ref.z);
	out.z = (this->x * A_ref.y) - (this->y * A_ref.x);
	return out;
}


float BRZ::Vec3::Dot(const BRZ::Vec3 & A_ref) const
{
	float dot = (this->x * A_ref.x) + (this->y * A_ref.y) + (this->z * A_ref.z);
	return dot;
}


float BRZ::Vec3::Length() const
{
	return sqrt(this->SqrLength());
}


float BRZ::Vec3::SqrLength() const
{
	float out = pow(x, 2) + pow(y, 2) + pow(z, 2);
	return out;
}


BRZ::Vec3 BRZ::Vec3::Normalize() const
{
	BRZ::Vec3 out;
	if (!(*this))
		return out;

	float fac = this->Length();
	out.x = x / fac;
	out.y = y / fac;
	out.z = z / fac;
	return out;
}