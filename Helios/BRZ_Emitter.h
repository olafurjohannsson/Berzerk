
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Emitter.h" )
#endif

#include "BRZ_GameDefine.h"

#include "BRZ_Actor.h"

namespace BRZ
{
	class Emitter : public BRZ::Actor
	{
	public:
		struct Description
		{
			Description() { BRZ::ZeroMem(this);	}

			signed int			lifeEmitter;
			signed int			lifeParticle;
			signed int			lifeSkew;
			float				genRate;
			float				genDecay;
			unsigned int		genPasses;
			
			float				colourSpread[3];
			float				colourSkew;
			BRZ::Colour			colour[4];

			float				angleOut;
			float				angleSkew;
			float				velocityOut;
			float				velocitySkew;

			float				dampenFactor;
			float				dampenPower;
		};

		struct Particle
		{
			Particle() { BRZ::ZeroMem(this);	}

			signed int	lifetimeMS;
			signed int	durationMS;

			BRZ::Vec2		position;
			BRZ::Vec2		velocity;
			
			float			colourSpread[3];
		}; 

	public:
		Emitter();
		~Emitter();

	public:
		// Actor virtual functions:
		virtual BRZRESULT	Construct(const BRZ::Asset & asset, BRZ::Display & display, BRZ::InputCore & input, const BRZ::Vec2 & position, float rotation);
		virtual BRZRESULT	Render();
		virtual BRZRESULT	Cycle(unsigned int deltaTimeMS);

		// Emitter specific actions:
		BRZRESULT	Engage(const Description & specs, std::ofstream & log);
		BRZRESULT	Reset();
		BRZRESULT	ToggleHelper();
		bool		IsComplete() const;

	private:
		std::ofstream *			log;
		bool					active;
		bool					helper;
		Description				specs;
		signed int				duration;
		signed int				delta;
		std::vector<Particle>	parts;
		BRZ::Display *			display;
	};
}