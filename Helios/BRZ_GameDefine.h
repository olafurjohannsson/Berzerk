
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_GameDefine.h" )
#endif

#include "BRZ_Define.h"
#include "BRZ_Input.h"

#include "BRZ_Colour.h"
#include "BRZ_Coord2.h"
#include "BRZ_Vec2.h"

// Forward declarations of game classes:
namespace BRZ
{
	class Environment;
	class Scene;
	class Asset;

	class Actor;
	struct ActorReference;

	class RigidObject;
	struct RigidElement;
	struct RigidLine;

	class Skeleton;

	struct Orientation
	{
		Orientation() : rotation(0)	{    }

		BRZ::Vec2		position;
		float			rotation;
	};

	namespace BONE
	{
		enum Craft
		{
			CRAFT_HULL			= 0,
			CRAFT_MAIN_THRUSTER,
			CRAFT_RIGHT_THRUSTER,
			CRAFT_LEFT_THRUSTER,
			CRAFT_RIGHT_RCS,
			CRAFT_LEFT_RCS,

			CRAFT_MAX_BONES
		};

		enum Default
		{
			DEFAULT				= 0,

			DEFAULT_MAX_BONES
		};
	}
}