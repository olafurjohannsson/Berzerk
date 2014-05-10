
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_RigidObject.h" )
#endif

#include "BRZ_GameDefine.h"

namespace BRZ
{
	struct RigidLine
	{
		BRZ::Vec2		point[2];
	};

	struct RigidElement
	{
		RigidElement() : lines(NULL), lineCount(0), radius(0), boundRadius(0), active(true)	{    }
		~RigidElement()	{ if (lines != NULL) { delete [] lines;	}	}

		bool Empty() const	{ return (lineCount == 0);	}

		BRZ::RigidLine *		lines;
		unsigned int			lineCount;
		float					radius;
		float					boundRadius;
		bool					active;
		BRZ::Orientation		local;
	};

	class RigidObject
	{
	public:
		RigidObject(const BRZ::Orientation & worldOrient);
		~RigidObject();

	public:
		BRZRESULT Construct(const BRZ::Asset & asset);

		BRZRESULT PlaceElement(unsigned int index, const BRZ::Vec2 & position);
		BRZRESULT RotateElement(unsigned int index, const float & rotation);
		BRZRESULT EnableElement(unsigned int index);
		BRZRESULT DisableElement(unsigned int index);

		inline float RealRadius() const		{ return radius; }
		inline float BoundRadius() const	{ return bounds; }

		bool	  TestCollision(const BRZ::RigidObject & other) const;

	protected:
		BRZRESULT RecalculateBounds();

	private:
		const BRZ::Orientation &	world;
		BRZ::RigidElement *			elements;
		unsigned int				elemCount;
		float						radius;
		float						bounds;
	};
}