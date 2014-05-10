
#include "BRZ_Actor.h"

#include "BRZ_Asset.h"
#include "BRZ_Display.h"

//	Note: Function passes of the task of testing for a collision with another
//			Actor to the RigidBody, which tests against the RigidBody of the
//			other Actor in question.  A return value of TRUE indicates that a
//			collision is taking place between the two.
//		  Observe that the RigidBody::TestCollision method is computationally
//			expensive and should not be called unless the two actors are within
//			bound radii, for best results.
bool BRZ::Actor::TestCollision(const BRZ::Actor & A_other) const
{
	return physical.TestCollision(A_other.physical);
}


//	Note: Accessor method, returns the bounding radius of the actor as defined
//			by the RigidBody.
unsigned int BRZ::Actor::BoundRadius() const
{
	return physical.BoundRadius();
}


//	Note: Stub function; Default Actor class has no behaviour.
BRZRESULT BRZ::Actor::Cycle(unsigned int A_dt)
{
	return BRZ_SUCCESS;
}


//	Note: Stub function; Default Actor class handles no input events.
BRZRESULT BRZ::Actor::ResolveInput(BRZ::InputEvent A_event)
{
	return BRZ_SUCCESS;
}


//	Note: Function must take care of rendering all LineObjects associated with
//			the actor that require drawing.
BRZRESULT BRZ::Actor::Render()
{
	return graphic.Render(location.position, location.rotation);
}


//	Note: Function is invoked on an Actor object to bring it to a usable state.  This method
//			must take care, at least, of linking the LineObject to a graphical resource,
//			of setting up the physical RigidBody for the object, and initializing the Skeleton.
BRZRESULT BRZ::Actor::Construct(const BRZ::Asset & A_asset, BRZ::Display & A_disp, BRZ::InputCore & A_input, \
								const BRZ::Vec2 & A_pos = BRZ::Vec2(), float A_rot = 0)
{
	location.position = A_pos;
	location.rotation = A_rot;

	A_disp.Link(graphic, A_asset.Name());
	physical.Construct(A_asset);
	skeleton.Construct(A_asset);

	return BRZ_SUCCESS;
}


BRZ::Actor::Actor() :
	physical(location),
	skeleton(location, graphic, physical)
{
}


BRZ::Actor::~Actor()
{
}