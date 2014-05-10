
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Actor.h" )
#endif

#include "BRZ_GameDefine.h"

#include "BRZ_LineObject.h"
#include "BRZ_RigidObject.h"
#include "BRZ_Skeleton.h"

namespace BRZ
{
	class Actor : public BRZ::ActiveObject
	{
	public:
		Actor();
		~Actor();

	public:
		virtual BRZRESULT		Construct(const BRZ::Asset & asset, BRZ::Display & display, BRZ::InputCore & input, const BRZ::Vec2 & position, float rotation);

		//	Note: General purpose functions called on a frame-by-frame basis, by the Scene
		//			as it cycles through the frame.  Note that ResolveInput is invoked as events
		//			are triggered by the InputCore.
		virtual BRZRESULT		Render();
		virtual BRZRESULT		ResolveInput(BRZ::InputEvent action);
		virtual BRZRESULT		Cycle(unsigned int deltaTimeMS);

		virtual unsigned int	BoundRadius() const;
		virtual bool			TestCollision(const BRZ::Actor & other) const;

	protected:
		BRZ::Orientation		location;
		BRZ::LineObject			graphic;
		BRZ::RigidObject		physical;
		BRZ::Skeleton			skeleton;
	};

	struct ActorReference
	{
		ActorReference() : ref(NULL)	{    }

		BRZ::Orientation		oldLocation;
		BRZ::Actor *			ref;
	};
}