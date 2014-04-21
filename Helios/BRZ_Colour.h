
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Colour.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Colour
	{
	public:
		Colour();
		Colour(float r, float g, float b, float a);
		Colour(const Colour & ref);
		~Colour();

	public:
		const float & operator[](unsigned int index) const;
		float & operator[](unsigned int index);

	public:
		union
		{
			float ptr[4];
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
		};
		

	private:
		static float BAD_INDEX;
	};
}