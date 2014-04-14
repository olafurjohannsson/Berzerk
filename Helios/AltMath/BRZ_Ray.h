
#pragma once

#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Ray.h")
#endif

#include "BRZ_Define.h"
#include "BRZ_Vec3.h"

namespace BRZ
{
	class Ray
	{
	public:
		Ray()			{	}
		Ray(const BRZ::Vec3 & origin, const BRZ::Vec3 & cast) : p0(origin), p1(cast)	{	}

	public:
		BRZ::Vec3		p0;
		BRZ::Vec3		p1;
	};
}
