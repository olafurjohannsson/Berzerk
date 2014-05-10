
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
			Reference() : base(NULL), visible (true), rotation(0)	{    }

		public:
			const BRZ::LineElement *	base;
			bool						visible;
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
		BRZRESULT ShowElement(unsigned int index);
		BRZRESULT HideElement(unsigned int index);
		BRZRESULT PlaceElement(unsigned int index, const BRZ::Vec2 & position);
		BRZRESULT RotateElement(unsigned int index, const float & rotation);

		
	private:
		BRZ::Display *		display;
		Reference *			elements;
		unsigned int		numElements;
	};
}