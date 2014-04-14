
#pragma once

#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Rect.h")
#endif

#include "BRZ_Define.h"

// Required for usage of RECT..
#include <Windows.h>

namespace BRZ
{
	class Rect
	{
	public:
		enum Corner;

	public:
		Rect() : left(0), right(0), upper(0), lower(0)		{		}

	public:
		inline bool operator ==(const BRZ::Rect & ref) const;
		inline bool operator !=(const BRZ::Rect & ref) const;
		inline Rect operator +(const BRZ::Coord2 & delta) const;
		inline Rect operator -(const BRZ::Coord2 & delta) const;
		inline void operator +=(const BRZ::Coord2 & delta);
		inline void operator -=(const BRZ::Coord2 & delta);
		inline bool operator !() const;
		inline bool Contains(const BRZ::Coord2 & point) const;

		inline BRZ::Coord2 Point(BRZ::Rect::Corner corner) const;
		inline BRZ::Coord2 Size() const;


		inline operator RECT () const
		{
			RECT out;
			out.left = left;
			out.right = right;
			out.top = upper;
			out.bottom = lower;

			return out;
		}

	public:
		signed int			left;
		signed int			right;
		signed int			upper;
		signed int			lower;

	public:
		enum Corner
		{
			TOPLEFT,
			TOPRIGHT,
			BTMLEFT,
			BTMRIGHT
		};
	};
}


BRZ::Coord2 BRZ::Rect::Size() const
{
	return BRZ::Coord2(right - left, lower - upper);
}


BRZ::Coord2 BRZ::Rect::Point(BRZ::Rect::Corner A_pt) const
{
	switch (A_pt)
	{
	case TOPLEFT:	return BRZ::Coord2(left, upper);
	case TOPRIGHT:	return BRZ::Coord2(right, upper);
	case BTMLEFT:	return BRZ::Coord2(left, lower);
	case BTMRIGHT:	return BRZ::Coord2(right, lower);
	}

	return BRZ::Coord2();
}



bool BRZ::Rect::operator ==(const BRZ::Rect & A_ref) const
{
	return (A_ref.left == this->left && \
		A_ref.right == this->right && \
		A_ref.upper == this->upper && \
		A_ref.lower == this->lower);
}


bool BRZ::Rect::operator !=(const BRZ::Rect & A_ref) const
{
	return !(*this == A_ref);
}


BRZ::Rect BRZ::Rect::operator +(const BRZ::Coord2 & delta) const
{
	BRZ::Rect out = *this;
	out.left += delta.x;
	out.right += delta.x;
	out.upper += delta.y;
	out.lower += delta.y;

	return out;
}


BRZ::Rect BRZ::Rect::operator -(const BRZ::Coord2 & delta) const
{
	BRZ::Rect out = *this;
	out.left -= delta.x;
	out.right -= delta.x;
	out.upper -= delta.y;
	out.lower -= delta.y;

	return out;
}


void BRZ::Rect::operator +=(const BRZ::Coord2 & delta)
{
	left += delta.x;
	right += delta.x;
	upper += delta.y;
	lower += delta.y;
}


void BRZ::Rect::operator -=(const BRZ::Coord2 & delta)
{
	left -= delta.x;
	right -= delta.x;
	upper -= delta.y;
	lower -= delta.y;
}


bool BRZ::Rect::operator !() const
{
	return (*this == BRZ::Rect());
}


bool BRZ::Rect::Contains(const BRZ::Coord2 & A_point) const
{
	if ((A_point.x < right) && A_point.x > left)
	{
		if ((A_point.y > upper) && A_point.y < lower)
			return true;
	}
	return false;
}
