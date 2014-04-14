
#pragma once

#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Triangle.h")
#endif

#include "BRZ_Define.h"

#include "BRZ_Ray.h"
#include "BRZ_Vec3.h"

namespace BRZ
{
	class Triangle
	{
	public:
		inline BRZ::Vec3 & operator [] (unsigned int index);
		inline const BRZ::Vec3 & operator [] (unsigned int index) const;

		inline bool Intersect(const BRZ::Ray & ray, BRZ::Vec3 & out) const;

	public:
		BRZ::Vec3		a;
		BRZ::Vec3		b;
		BRZ::Vec3		c;
	};
}


bool BRZ::Triangle::Intersect(const BRZ::Ray & A_ray, BRZ::Vec3 & A_out) const
{
	const float FP_EPS = 0.00001f;

	BRZ::Vec3 triU;
	BRZ::Vec3 triV;
	BRZ::Vec3 triN;

	BRZ::Vec3 rDir;
	BRZ::Vec3 w0;
	BRZ::Vec3 w;

	float fr;
	float fa;
	float fb;


	triU = b - a;
	triV = c - a;
	triN = triU.Cross(triV);

	if (triN.SqrLength() == 0)
		return false;

	rDir = A_ray.p1 - A_ray.p0;
	w0 = A_ray.p0 - a;
	fa = triN.Dot(w0) * -1.0f;
	fb = triN.Dot(rDir);

	if (fabs(fb) < FP_EPS)
	{
		if (fabs(fa) < FP_EPS)
			return false;	
		else
			return false;
	}

	fr = fa / fb;
	if (fr < 0.0f || fr > 1.0f)
		return false;

	A_out = A_ray.p0 + (rDir * fr);

	// Clamp near-zero values to 0:
	if (fabs(A_out.x) < 0.0001)
		A_out.x = 0;
	if (fabs(A_out.y) < 0.0001)
		A_out.y = 0;
	if (fabs(A_out.z) < 0.0001)
		A_out.z = 0;

	float uu;
	float uv;
	float vv;
	float wu;
	float wv;
	float D;

	uu = triU.Dot(triU);
	uv = triU.Dot(triV);
	vv = triV.Dot(triV);

	w = A_out - a;

	wu = w.Dot(triU);
	wv = w.Dot(triV);
	D = (uv * uv) - (uu * vv);

	float s;
	float t;

	s = ((uv * wv) - (vv * wu)) / D;
	if (s < 0.0f || s > 1.0f)
		return false;
	
	t = ((uv * wu) - (uu * wv)) / D;
	if (t < 0.0f || (s + t) > 1.0f)
		return false;

	return true;
}


BRZ::Vec3 & BRZ::Triangle::operator [] (unsigned int A_idx)
{
	switch (A_idx)
	{
	case 0:			return a;
	case 1:			return b;
	case 2:			return c;
	default:		return a;
	}
}


const BRZ::Vec3 & BRZ::Triangle::operator [] (unsigned int A_idx) const
{
	switch (A_idx)
	{
	case 0:			return a;
	case 1:			return b;
	case 2:			return c;
	default:		return a;
	}
}



