
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_AsteroidGen.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class AsteroidGen
	{
	public:
		BRZRESULT MakeRock(BRZ::RawGeometry & output);
		BRZRESULT MakeRockB(BRZ::RawGeometry & output);
	};
}