
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Vec2.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Vec2
	{
	public:
		Vec2();
		Vec2(float x, float y);
		Vec2(const Vec2 & ref);
		~Vec2();

	public:
		const float & operator[](unsigned int index) const;
		float & operator[](unsigned int index);

		BRZ::Vec2 operator+(const BRZ::Vec2 & add) const;
		BRZ::Vec2 operator-(const BRZ::Vec2 & subtract) const;
		BRZ::Vec2 operator*(const float & scalar) const;

		const BRZ::Vec2 & operator+=(const BRZ::Vec2 & add);
		const BRZ::Vec2 & operator-=(const BRZ::Vec2 & subtract);
		const BRZ::Vec2 & operator*=(const float & scalar);

		float Length() const;
		float SqrLength() const;
		BRZ::Vec2 Normalize() const;
		BRZ::Vec2 Rotate(float theta) const;

	public:
		// DiskStream utility functions:
		BRZRESULT		Unpack(BRZ::DiskStream & inputFile);
		unsigned int	Pack(BRZ::DiskStream & outputFile) const;

	public:
		float x;
		float y;

	private:
		static float BAD_INDEX;
	};
}