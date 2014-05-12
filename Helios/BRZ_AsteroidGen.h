
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_AsteroidGen.h" )
#endif

#include "BRZ_GameDefine.h"

namespace BRZ
{
	class AsteroidGen
	{
	public:
		AsteroidGen(const BRZ::Config & A_cfg) : cfg(A_cfg), fractalDepth(0)		{    }

	public:
		BRZRESULT MakeGenericRock(BRZ::RawGeometry & output);


		BRZRESULT MakeFractalRock(BRZ::Asset & output, float radius, unsigned int surfaces);
		BRZRESULT SubDivide(const BRZ::Vec2 & pointA, float radA, const BRZ::Vec2 & pointB, float radB, std::vector<BRZ::Vec2> & ptList);

	private:
		const BRZ::Config &		cfg;
		unsigned int			fractalDepth;


		static unsigned int		counter;
	};
}