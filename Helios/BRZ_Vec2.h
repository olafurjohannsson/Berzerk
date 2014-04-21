
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

	public:
		float x;
		float y;

	private:
		static float BAD_INDEX;
	};
}