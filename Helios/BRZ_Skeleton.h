
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Skeleton.h" )
#endif

#include "BRZ_GameDefine.h"

namespace BRZ
{
	//	Note: Skeleton class coordinates movement of actor sub-objects (elements)
	//			for both the LineObject and RigidObject, and any others that need
	//			be aware of specific positioning of the actors sub-elements.
	//		  In this way the actor needs only to invoke methods on the skeleton such
	//			as "MoveNode(BONE, offset)" or "PlaceNode(BONE, position, rotation)" 
	//			or even "Show/HideNode(BONE)", and the changes will be propogated to
	//			the objects these changes effect.
	class Skeleton
	{
	public:
		Skeleton(const BRZ::Orientation & worldOrient, BRZ::LineObject & lineObj, BRZ::RigidObject & rigidObj);
		~Skeleton();

	public:
		// Function to initialize the skeleton to represent an asset:
		BRZRESULT Construct(const BRZ::Asset & asset);

		// Functions for changing the state of specific bones:
		BRZRESULT MoveBone(unsigned int bone, const BRZ::Vec2 & offset);
		BRZRESULT RotateBone(unsigned int bone, float rotation);
		BRZRESULT PlaceBone(unsigned int bone, const BRZ::Vec2 & position);
		BRZRESULT PlaceBone(unsigned int bone, float rotation);
		BRZRESULT PlaceBone(unsigned int bone, const BRZ::Vec2 & position, float rotation);
		BRZRESULT PlaceBone(unsigned int bone, const BRZ::Orientation & orient);
		BRZRESULT CenterBone(unsigned int bone);
		BRZRESULT ShowBone(unsigned int bone);
		BRZRESULT HideBone(unsigned int bone);

		// Functions for accessing the state of specific bones:
		bool				BoneVisible(unsigned int bone) const;
		BRZ::Vec2			BonePosition(unsigned int bone) const;
		float				BoneRotation(unsigned int bone) const;
		BRZ::Orientation	BoneOrientation(unsigned int bone) const;

	private:
		const BRZ::Orientation &	world;
		BRZ::Orientation *			bones;
		BRZ::Orientation *			origins;
		bool *						visible;
		unsigned int				count;
		BRZ::LineObject &			lineRef;
		BRZ::RigidObject &			rigidRef;
	};
}