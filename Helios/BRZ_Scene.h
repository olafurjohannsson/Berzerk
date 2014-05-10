
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Scene.h" )
#endif

#include "BRZ_GameDefine.h"

namespace BRZ
{
	typedef std::list<BRZ::ActorReference *>			ActorRefList;
	typedef std::list<BRZ::Actor *>						ActorList;
	typedef std::map<BRZSTRING, BRZ::RigidObject *>		RigidArchive;

	class Scene
	{
	public:
		Scene(BRZ::Display & video);
		~Scene();

	public:
		BRZRESULT	SetUp();
		BRZRESULT	Cycle(unsigned int deltaTimeMS);
		BRZRESULT	Reset();

		BRZRESULT	LoadBounds(const BRZ::RawGeometry & geometry, const BRZSTRING & name);

	protected:

	private:
		// Engine objects to interact with:
		BRZ::Display &			video;
		BRZ::InputCore &		input;

		// Internal storage of rigid body templates:
		BRZ::RigidArchive		rigidBodies;

		// Actor storage, current frame and last frame;
		BRZ::ActorRefList		lastActors;
		BRZ::ActorList			nowActors;
	};
}