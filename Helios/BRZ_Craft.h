
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Craft.h" )
#endif

#include "BRZ_GameDefine.h"

#include "BRZ_Actor.h"

namespace BRZ
{
	class Craft : public BRZ::Actor
	{
	public:
		Craft(const BRZ::Config & cfg);
		~Craft();

	public:
		virtual BRZRESULT	Construct(const BRZ::Asset & asset, BRZ::Display & display, BRZ::InputCore & input, const BRZ::Vec2 & position = BRZ::Vec2(), float rotation = 0);
		virtual BRZRESULT	ResolveInput(BRZ::InputEvent action);
		virtual BRZRESULT	Cycle(unsigned int deltaTimeMS);

		// TESTING function.  Resets position and randomizes rotation.
		BRZRESULT			Reset();

	protected:
		BRZ::BindingModule *	binds;

		float					mainThrust;
		float					angularThrust;
		float					rcsThrust;

		float					mainDamp;
		float					angularDamp;

		bool					thrustState[4];
		BRZ::Vec2				velocity;
		float					angularVelocity;
	};
}