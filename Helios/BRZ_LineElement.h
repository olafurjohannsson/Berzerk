
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_LineElement.h" )
#endif

#include "BRZ_Define.h"

#include "BRZ_Colour.h"
#include "BRZ_Vec2.h"

namespace BRZ
{
	class LineElement
	{
	public:
		LineElement();
		LineElement(const LineElement & ref);
		~LineElement();

	public:
		BRZ::Colour		colour;
		unsigned int	ptOffset;
		unsigned int	ptCount;
		unsigned int	idxOffset;
		unsigned int	idxCount;
		BRZ::Vec2		localPos;

	protected:
	private:
	};
}