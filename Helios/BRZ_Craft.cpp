
#include "BRZ_Craft.h"

#include "BRZ_Asset.h"
#include "BRZ_BindingModule.h"
#include "BRZ_Config.h"
#include "BRZ_Display.h"


//	Note: TESTING FUNCTION.  See declaration.
BRZRESULT BRZ::Craft::Reset()
{
	location.position = BRZ::Vec2(0, 0);

	unsigned int	factor = ::rand() % 1000;
	float			rot = factor;

	rot *= (0.001 * BRZ::TWO_PI);

	location.rotation = rot;

	velocity = BRZ::Vec2(0, 0);
	angularVelocity = 0;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Craft::Cycle(unsigned int A_dt)
{
	float ms = ((float)A_dt) * 0.001f;

	bool	dampMain = true;
	bool	dampAng = true;
	BRZ::Vec2	mainVector(0, 1);
	BRZ::Vec2	rcsVector(0, -1);

	if (thrustState[0])
	{
		skeleton.ShowBone(BRZ::BONE::CRAFT_MAIN_THRUSTER);
		velocity += mainVector.Rotate(location.rotation) * (ms * mainThrust);
		thrustState[0] = false;
		dampMain = false;
	}
	else
	{
		skeleton.HideBone(BRZ::BONE::CRAFT_MAIN_THRUSTER);
	}


	if (thrustState[1])
	{
		skeleton.ShowBone(BRZ::BONE::CRAFT_LEFT_THRUSTER);
		angularVelocity -= (angularThrust * ms);
		thrustState[1] = false;
		dampAng = false;
	}
	else
	{
		skeleton.HideBone(BRZ::BONE::CRAFT_LEFT_THRUSTER);
	}


	if (thrustState[2])
	{
		skeleton.ShowBone(BRZ::BONE::CRAFT_RIGHT_THRUSTER);
		angularVelocity += (angularThrust * ms);
		thrustState[2] = false;
		dampAng = false;
	}
	else
	{
		skeleton.HideBone(BRZ::BONE::CRAFT_RIGHT_THRUSTER);
	}


	if (thrustState[3])
	{
		skeleton.ShowBone(BRZ::BONE::CRAFT_RIGHT_RCS);
		skeleton.ShowBone(BRZ::BONE::CRAFT_LEFT_RCS);
		velocity += rcsVector.Rotate(location.rotation) * (ms * rcsThrust);
		thrustState[3] = false;
		dampMain = false;
	}
	else
	{
		skeleton.HideBone(BRZ::BONE::CRAFT_RIGHT_RCS);
		skeleton.HideBone(BRZ::BONE::CRAFT_LEFT_RCS);
	}

	// If neither main thruster nor RCS thrusters engaged, dampen velocity:
	if (dampMain && velocity.Length() != 0)
	{
		BRZ::Vec2 dampVector = velocity.Normalize() * (ms * mainDamp * -1);

		if (dampVector.Length() >= velocity.Length())
			velocity = BRZ::Vec2(0, 0);
		else
		{
			velocity += dampVector;
		}
	}

	// If neither left nor right thrusters engaged, dampen angular velocity:
	if (dampAng && angularVelocity != 0)
	{
		if (angularVelocity > 0)
		{
			float dampSize = (ms * angularDamp);

			if (dampSize > angularVelocity)
				angularVelocity = 0;
			else
				angularVelocity -= dampSize;
		}
		else
		{
			float dampSize = (ms * angularDamp * -1);

			if (dampSize < angularVelocity)
				angularVelocity = 0;
			else
				angularVelocity -= dampSize;
		}
	}

	// Finally update craft orientation based on velocity and angular velocity:
	location.position += velocity;
	location.rotation = BRZ::ClampRotation(location.rotation + angularVelocity);

	// SMALL HACK:	If craft travels outside the WINDOWED MODE screen (800 x 600), make
	//					it come back on the other side:
	if (location.position.x > (400 + physical.BoundRadius()))
		location.position.x = (-400 - physical.RealRadius());
	else if (location.position.x < (-400 - physical.BoundRadius()))
		location.position.x = (400 + physical.RealRadius());

	if (location.position.y > (300 + physical.BoundRadius()))
		location.position.y = (-300 - physical.RealRadius());
	else if (location.position.y < (-300 - physical.BoundRadius()))
		location.position.y = (300 + physical.RealRadius());

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Craft::ResolveInput(BRZ::InputEvent A_event)
{
	switch (A_event)
	{
	case BRZ::IE_MAIN_THRUSTER:
		thrustState[0] = true;
		return BRZ_SUCCESS;

	case BRZ::IE_LEFT_THRUSTER:
		thrustState[1] = true;
		return BRZ_SUCCESS;

	case BRZ::IE_RIGHT_THRUSTER:
		thrustState[2] = true;
		return BRZ_SUCCESS;

	case BRZ::IE_RCS_THRUSTER:
		thrustState[3] = true;
		return BRZ_SUCCESS;
	}

	return BRZ_FAILURE;
}


BRZRESULT BRZ::Craft::Construct(const BRZ::Asset & A_asset, BRZ::Display & A_disp, BRZ::InputCore & A_input, \
								const BRZ::Vec2 & A_pos, float A_rot)
{
	location.position = A_pos;
	location.rotation = A_rot;

	A_disp.Link(graphic, A_asset.Name());
	physical.Construct(A_asset);
	skeleton.Construct(A_asset);

	skeleton.HideBone(BRZ::BONE::CRAFT_MAIN_THRUSTER);
	skeleton.HideBone(BRZ::BONE::CRAFT_RIGHT_THRUSTER);
	skeleton.HideBone(BRZ::BONE::CRAFT_LEFT_THRUSTER);
	skeleton.HideBone(BRZ::BONE::CRAFT_RIGHT_RCS);
	skeleton.HideBone(BRZ::BONE::CRAFT_LEFT_RCS);

	binds = new BRZ::BindingModule(A_input);
	binds->Link(this);
	binds->Listen(BRZ::IE_MAIN_THRUSTER);
	binds->Listen(BRZ::IE_LEFT_THRUSTER);
	binds->Listen(BRZ::IE_RIGHT_THRUSTER);
	binds->Listen(BRZ::IE_RCS_THRUSTER);
	binds->Engage();

	return BRZ_SUCCESS;
}


BRZ::Craft::Craft(const BRZ::Config & A_cfg) : 
	binds(NULL),
	mainThrust(A_cfg.ReadFloat(L"CraftMainThrust")),
	angularThrust(BRZ::DegreeToRad(A_cfg.ReadFloat(L"CraftRThrust"))),
	rcsThrust(A_cfg.ReadFloat(L"CraftRCSThrust")),
	mainDamp(A_cfg.ReadFloat(L"CraftDampening")),
	angularDamp(BRZ::DegreeToRad(A_cfg.ReadFloat(L"CraftRDamp"))),
	velocity(0, 0),
	angularVelocity(0)
{
	BRZ::ZeroMem(thrustState, 4);
}


BRZ::Craft::~Craft()
{
	if (binds != NULL)
		delete binds;
}