
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_RawElement.h"
#endif

#include "BRZ_Define.h"

#include "BRZ_Colour.h"
#include "BRZ_Vec2.h"

namespace BRZ
{
	class RawElement
	{
	public:
		RawElement();
		RawElement(const RawElement & ref);
		~RawElement();

	public:
		void AllocPoints(unsigned int count);
		void AllocLines(unsigned int count);

	public:
		BRZ::Colour		colour;
		BRZ::Vec2 *		points;
		BRZ::Coord2 *	lines;
		BRZ::Vec2		offset;

		unsigned int	pointCount;
		unsigned int	lineCount;
	};
}