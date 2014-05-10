
#include "BRZ_Skeleton.h"

#include "BRZ_Asset.h"
#include "BRZ_LineObject.h"
#include "BRZ_RigidObject.h"


BRZ::Orientation BRZ::Skeleton::BoneOrientation(unsigned int A_bone) const
{
	if (A_bone >= count)
		return BRZ::Orientation();

	return bones[A_bone];
}


float BRZ::Skeleton::BoneRotation(unsigned int A_bone) const
{
	if (A_bone >= count)
		return 0;

	return bones[A_bone].rotation;
}


BRZ::Vec2 BRZ::Skeleton::BonePosition(unsigned int A_bone) const
{
	if (A_bone >= count)
		return BRZ::Vec2(0, 0);

	return bones[A_bone].position;
}


bool BRZ::Skeleton::BoneVisible(unsigned int A_bone) const
{
	if (A_bone >= count)
		return false;

	return visible[A_bone];
}


//	Note: Function disables collision detection and rendering for the specified
//			bone.
//		  Observe that if the RigidElement corresponding to the bone is non-rigid
//			this function will not change that.
BRZRESULT BRZ::Skeleton::HideBone(unsigned int A_bone)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	visible[A_bone] = false;

	lineRef.HideElement(A_bone);
	rigidRef.DisableElement(A_bone);

	return BRZ_SUCCESS;
}


//	Note: Function enables collision detection and rendering for the specified
//			bone.
//		  Observe that if the RigidElement corresponding to the bone is non-rigid
//			this function will not change that, and it will still not collide.
BRZRESULT BRZ::Skeleton::ShowBone(unsigned int A_bone)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	visible[A_bone] = true;

	lineRef.ShowElement(A_bone);
	rigidRef.EnableElement(A_bone);

	return BRZ_SUCCESS;
}


//	Note: Function returns the specified bone to its original position and rotation
//			based on where it was constructed as defined by the asset used to construct it.
BRZRESULT BRZ::Skeleton::CenterBone(unsigned int A_bone)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	return this->PlaceBone(A_bone, origins[A_bone].position, origins[A_bone].rotation);
}


BRZRESULT BRZ::Skeleton::PlaceBone(unsigned int A_bone, const BRZ::Orientation & A_orient)
{
	return this->PlaceBone(A_bone, A_orient.position, A_orient.rotation);
}


//	Note: Function PLACES the specific bone at the rotation and position passed in, and
//			propogates the changes through the LineObject and RigidObject.
BRZRESULT BRZ::Skeleton::PlaceBone(unsigned int A_bone, const BRZ::Vec2 & A_pos, float A_rot)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	A_rot = BRZ::ClampRotation(A_rot);

	bones[A_bone].position = A_pos;
	bones[A_bone].rotation = A_rot;

	lineRef.PlaceElement(A_bone, A_pos);
	lineRef.RotateElement(A_bone, A_rot);

	rigidRef.PlaceElement(A_bone, A_pos);
	rigidRef.RotateElement(A_bone, A_rot);

	return BRZ_SUCCESS;
}


//	Note: Function PLACES the specific bone at the rotation passed in, and propogates
//			the changes through the LineObject and RigidObject.
BRZRESULT BRZ::Skeleton::PlaceBone(unsigned int A_bone, float A_rot)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	A_rot = BRZ::ClampRotation(A_rot);

	bones[A_bone].rotation = A_rot;

	lineRef.RotateElement(A_bone, A_rot);
	rigidRef.RotateElement(A_bone, A_rot);

	return BRZ_SUCCESS;
}


//	Note: Function PLACES the specific bone at the position passed in, and propogates
//			the changes through the LineObject and RigidObject.
BRZRESULT BRZ::Skeleton::PlaceBone(unsigned int A_bone, const BRZ::Vec2 & A_pos)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	bones[A_bone].position = A_pos;

	lineRef.PlaceElement(A_bone, A_pos);
	rigidRef.PlaceElement(A_bone, A_pos);

	return BRZ_SUCCESS;
}


//	Note: Function rotates the specific bone by the offset, and propogates the changes
//			through the LineObject and RigidObject.  Note that this rotation is 
//			relative to the current bone rotation.
BRZRESULT BRZ::Skeleton::RotateBone(unsigned int A_bone, float A_rot)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	bones[A_bone].rotation = BRZ::ClampRotation(bones[A_bone].rotation + A_rot);

	lineRef.RotateElement(A_bone, bones[A_bone].rotation);
	rigidRef.RotateElement(A_bone, bones[A_bone].rotation);

	return BRZ_SUCCESS;
}


//	Note: Function moves the specific bone by the offset, and propogates the changes
//			through the LineObject and RigidObject.
BRZRESULT BRZ::Skeleton::MoveBone(unsigned int A_bone, const BRZ::Vec2 & offset)
{
	if (A_bone >= count)
		return BRZ_FAILURE;

	bones[A_bone].position += offset;

	lineRef.PlaceElement(A_bone, bones[A_bone].position);
	rigidRef.PlaceElement(A_bone, bones[A_bone].position);

	return BRZ_SUCCESS;
}


//	Note: Function constructs a bone for each component present in the asset, where
//			a bone consists of a position and a rotation.  These are considered to be
//			the DEFINITIVE orientations of all the elements possessed by an actor, and
//			the Skeleton object itself sees to propogating changes in bone position and
//			rotation down through the LineObject and RigidObject.
//		  Observe that the Skeleton object does not exist in a usable state until this
//			method has been invoked -- it is generally called as part of an Actor's
//			"Construct(...)" method.
BRZRESULT BRZ::Skeleton::Construct(const BRZ::Asset & A_asset)
{
	// Ensure the Skeleton is not already constructed internally:
	if (bones != NULL)
		return BRZ_FAILURE;

	count = A_asset.NumElements();

	if (count == 0)
		return BRZ_SUCCESS;

	// Create the bones, initialize their positions to those specified by the asset
	//	and initialize them all to be visible (rendering, and colliding where rigid.)
	bones = new BRZ::Orientation[count];
	origins = new BRZ::Orientation[count];
	visible = new bool[count];

	for (unsigned int i = 0; i != count; ++i)
	{
		bones[i].position = A_asset.ElementPosition(i);
		origins[i].position = A_asset.ElementPosition(i);
		visible[i] = true;
	}

	return BRZ_SUCCESS;
}


BRZ::Skeleton::Skeleton(const BRZ::Orientation & A_world, BRZ::LineObject & A_lObj, BRZ::RigidObject & A_rObj) :
	world(A_world),
	bones(NULL),
	visible(NULL),
	count(0),
	lineRef(A_lObj),
	rigidRef(A_rObj)
{
}


BRZ::Skeleton::~Skeleton()
{
	if (bones != NULL)
		delete [] bones;

	if (visible != NULL)
		delete [] visible;
}