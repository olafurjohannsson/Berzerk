
#include "BRZ_Emitter.h"

#include "BRZ_Asset.h"
#include "BRZ_Display.h"


BRZRESULT BRZ::Emitter::Reset()
{
	active = false;
	duration = 0;
	delta = 0;
	parts.clear();

	return BRZ_SUCCESS;
}


bool BRZ::Emitter::IsComplete() const
{
	return (duration > (specs.lifeEmitter + delta));
}


BRZRESULT BRZ::Emitter::ToggleHelper()
{
	if (helper)
	{
		skeleton.HideBone(BRZ::BONE::DEFAULT);
		helper = false;
	}
	else
	{
		skeleton.ShowBone(BRZ::BONE::DEFAULT);
		helper = true;
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Emitter::Engage(const Description & A_specs, std::ofstream & A_log)
{
	if (active)
		return BRZ_FAILURE;

	// Copy the emitter description over, and enforce sanity upon its members:
	BRZ::CopyMem(&specs, &A_specs);

	if (specs.lifeEmitter < 0)
		specs.lifeEmitter = 0;

	if (specs.lifeParticle < 0)
		specs.lifeParticle = 0;

	if (specs.lifeSkew < 0)
		specs.lifeSkew = 0;


	active = true;
	log = &A_log;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Emitter::Cycle(unsigned int A_dt)
{
	if (!active)
		return BRZ_SUCCESS;
	else if (this->IsComplete())
		return BRZ_SUCCESS;

	float	dt = static_cast<float>(A_dt) * 0.001f;

	// Update all current particles:
	for (auto i = parts.begin(); i != parts.end(); ++i)
	{
		i->position += (i->velocity * dt);
		i->durationMS += A_dt;
	}

	duration += A_dt;

	// Determine if new particles require generation:
	float lifeMax = static_cast<float>(specs.lifeEmitter);
	float lifeNow = static_cast<float>(duration);
	float lifeSpread = lifeNow / lifeMax;

	if (lifeSpread > 1)
		lifeSpread = 1;

	for (unsigned int i = 0; i < specs.genPasses; ++i)
	{
		if (BRZ::RandomFloat(0, 1) < pow((1 - lifeSpread), 2) * specs.genRate)
		{
			Particle		newPart;
			signed int		newSkew = static_cast<signed int>(rand() % (specs.lifeSkew * 2)) - specs.lifeSkew;

			// If due to the life skew of the particle, it would have a negative lifetime, discard it:
			if (specs.lifeParticle + newSkew <= 0)
				continue;

			newPart.lifetimeMS = specs.lifeParticle + newSkew;
			newPart.position = location.position;
			
			float			newAngle = location.rotation + specs.angleOut + BRZ::RandomFloat((specs.angleSkew * -1), specs.angleSkew);
			float			newVelocity = specs.velocityOut + BRZ::RandomFloat((specs.velocitySkew * -1), specs.velocitySkew);
			BRZ::Vec2		unitVec = BRZ::Vec2(0, 1);

			newPart.velocity = unitVec.Rotate(newAngle) * newVelocity;

			newPart.colourSpread[0] = specs.colourSpread[0];
			newPart.colourSpread[1] = specs.colourSpread[1];
			newPart.colourSpread[2] = specs.colourSpread[2];

			parts.push_back(newPart);

			signed int lifeComp = newPart.lifetimeMS - (specs.lifeEmitter - duration);

			if (lifeComp > delta)
				delta = lifeComp;
		}
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Emitter::Render()
{
	if (!active)
		return BRZ_SUCCESS;

	graphic.Render(location.position, location.rotation);

	for (auto i = parts.cbegin(); i != parts.cend(); ++i)
	{
		if (i->durationMS > i->lifetimeMS)
			continue;

		// Particle position is known, but the colour needs to be generated
		//	from the colour spread and duration of the particle:

		float	lifeMax = static_cast<float>(i->lifetimeMS);
		float	lifeNow = static_cast<float>(i->durationMS);
		float	lifeSpread = lifeNow / lifeMax;

		BRZ::Colour	colour;

		for (unsigned int j = 0; j <= 3; ++j)
		{
			if (j == 3)
			{
				// The particle is outside its theoretical lifespan for some reason; colour
				//	is fixed to the final particle colour:
				colour = this->specs.colour[3];
				break;
			}

			if (lifeSpread < i->colourSpread[j])
			{
				BRZ::Colour		base = this->specs.colour[j];
				BRZ::Colour		mod = this->specs.colour[j + 1];
				float			factor = 0;

				if (j == 0)
					factor = lifeSpread / i->colourSpread[j];
				else
					factor = (lifeSpread - i->colourSpread[j - 1]) / (i->colourSpread[j] - i->colourSpread[j - 1]);

				colour.r = (base.r * (1 - factor)) + (mod.r * factor);
				colour.g = (base.g * (1 - factor)) + (mod.g * factor);
				colour.b = (base.b * (1 - factor)) + (mod.b * factor);
				colour.a = (base.a * (1 - factor)) + (mod.a * factor);

				break;
			}
		}

		display->QueueParticle(i->position, colour);
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Emitter::Construct(const BRZ::Asset & A_asset, BRZ::Display & A_disp, BRZ::InputCore & A_input, \
								  const BRZ::Vec2 & A_pos, float A_rot)
{
	display = &A_disp;

	location.position = A_pos;
	location.rotation = A_rot;

	A_disp.Link(graphic, A_asset.Name());
	physical.Construct(A_asset);
	skeleton.Construct(A_asset);

	// Helper defaults to invisible:
	skeleton.HideBone(BRZ::BONE::DEFAULT);

	return BRZ_SUCCESS;
}


BRZ::Emitter::Emitter() :
	log(NULL),
	active(false),
	helper(false),
	duration(0),
	delta(0)
{
}


BRZ::Emitter::~Emitter()
{
}