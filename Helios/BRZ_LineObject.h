
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_LineObject.h" )
#endif

#include "BRZ_Define.h"

#include "BRZ_Colour.h"
#include "BRZ_Vec2.h"

namespace BRZ
{
	class LineObject
	{
	public:
		struct Reference
		{
		public:
			const BRZ::LineElement *	base;
			bool						standard;
			BRZ::Colour					colour;
			BRZ::Vec2					offset;
			float						rotation;
		};

		friend class BRZ::Display;

	public:
		LineObject();
		~LineObject();

	public:
		BRZRESULT Render(const BRZ::Vec2 & position, float rotation);

	private:
		BRZ::Display *		display;
		Reference *			elements;
		unsigned int		numElements;
	};
}