
#include "BRZ_RigidObject.h"

#include "BRZ_Asset.h"

//	Note: Stub function -- TO BE IMPLEMENTED.
bool BRZ::RigidObject::TestCollision(const BRZ::RigidObject & A_other) const
{
	BRZ::Vec2	rootTransform = A_other.world.position - this->world.position;

	for (unsigned int i = 0; i < elemCount; ++i)
	{
		// If the source element is either empty (non-colliding) or inactive (hidden), it
		//	need not be tested.
		if (this->elements[i].Empty() || !this->elements[i].active)
			continue;


		for (unsigned int j = 0; j < A_other.elemCount; ++j)
		{
			// If the destination element is either empty (non-colliding) or inactive (hidden),
			//	it need not be tested against.
			if (A_other.elements[j].Empty() || !A_other.elements[j].active)
				continue;

			const BRZ::RigidElement & src = this->elements[i];
			const BRZ::RigidElement & dest = A_other.elements[j];

			// Transform the position of the elements to relative world space:
			BRZ::Vec2	srcPos = src.local.position.Rotate(this->world.rotation);
			BRZ::Vec2	destPos = rootTransform + dest.local.position.Rotate(A_other.world.rotation);

			// If other element + radius is not within radius of this element, no collision.
			float		distance = (destPos - srcPos).Length();
			if (distance > src.radius + dest.radius)
				continue;

			// It is clear the two elements are within a radius' length of one another, now we
			//	must test their lines individually for collision.
			for (unsigned int k = 0; k < src.lineCount; ++k)
			{
				for (unsigned int l = 0; l < dest.lineCount; ++l)
				{
					BRZ::Vec2	p[4];
					// Transform each line to its world space position:
					//	src.a -> p[0]
					//	src.b -> p[1]
					//	dest.a -> p[2]
					//	dest.b -> p[3]

					p[0] = srcPos + src.lines[k].point[0].Rotate(src.local.rotation + this->world.rotation);
					p[1] = srcPos + src.lines[k].point[1].Rotate(src.local.rotation + this->world.rotation);
					p[2] = destPos + dest.lines[l].point[0].Rotate(dest.local.rotation + A_other.world.rotation);
					p[3] = destPos + dest.lines[l].point[1].Rotate(dest.local.rotation + A_other.world.rotation);

					// Perform a line intersection test:
					float num_ua = ((p[3].x - p[2].x) * (p[0].y - p[2].y)) - ((p[3].y - p[2].y) * (p[0].x - p[2].x));
					float num_ub = ((p[1].x - p[0].x) * (p[0].y - p[2].y)) - ((p[1].y - p[0].y) * (p[0].x - p[2].x));
					float ua = 0.0f;
					float ub = 0.0f;

					float denom = ((p[3].y - p[2].y) * (p[1].x - p[0].x)) - ((p[3].x - p[2].x) * (p[1].y - p[0].y));

					// Test if the lines are coincident or parallel.
					// NOTE: At this point in time this registers as a non-collision, however this will need to
					//			be improved in future builds to handle these cases.
					if (denom == 0.0f)
					{
						// Lines are coincedent:
						if (num_ua == 0.0f && num_ub == 0.0f)
							continue;

						// lines are parallel:
						continue;
					}

					ua = num_ua / denom;
					ub = num_ub / denom;
	

					if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
					{
						// Get the intersection point.
						// A_impact.x = A_obj.p0.x + (ua * (A_obj.p1.x - A_obj.p0.x));
						// A_impact.y = A_obj.p0.y + (ua * (A_obj.p1.y - A_obj.p0.y));

						return true;
					}
				}
			}
		}
	}

	// No lines intersected..  What a waste of calculation.  Ehhhh.
	return false;
}


//	Note: Deactivates an element (specified by the index) for collision detection, 
//			and recalculates bounds:
BRZRESULT BRZ::RigidObject::DisableElement(unsigned int A_idx)
{
	if (A_idx >= elemCount)
		return BRZ_FAILURE;

	elements[A_idx].active = false;
	
	this->RecalculateBounds();

	return BRZ_SUCCESS;
}


//	Note: Activates an element (specified by the index) for collision detection, 
//			and recalculates bounds:
BRZRESULT BRZ::RigidObject::EnableElement(unsigned int A_idx)
{
	if (A_idx >= elemCount)
		return BRZ_FAILURE;

	elements[A_idx].active = true;
	
	this->RecalculateBounds();

	return BRZ_SUCCESS;
}


//	Note: Rotates an element(specified by the index) to the rotation indicated.  Note
//			that bounds need not be recalculated in this instance since changing the
//			rotation of an element does not effect the radius of its furthest point.
//		  Observe also that this is NOT a relative rotation, rather it is absolute,
//			the element's rotation is fixed to the rotation passed in.
BRZRESULT BRZ::RigidObject::RotateElement(unsigned int A_idx, const float & A_rot)
{
	if (A_idx >= elemCount)
		return BRZ_FAILURE;

	elements[A_idx].local.rotation = BRZ::ClampRotation(A_rot);

	return BRZ_SUCCESS;
}


//	Note: Places an element (specified by the index) at the local position indicated,
//			and recalculates the bounds based on the new conformation.
BRZRESULT BRZ::RigidObject::PlaceElement(unsigned int A_idx, const BRZ::Vec2 & A_pos)
{
	if (A_idx >= elemCount)
		return BRZ_FAILURE;

	elements[A_idx].local.position = A_pos;

	this->RecalculateBounds();

	return BRZ_SUCCESS;
}


//	Note: Determine the radius of the RigidObject in its current state.  Note that
//			the radius is equivalent to the furthest point of any element.  The bounds
//			in turn are defined as the radius + padding.  Note that only active and
//			and non-empty elements contribute to the radius and bounds.
BRZRESULT BRZ::RigidObject::RecalculateBounds()
{
	radius = 0;
	bounds = 0;

	for (unsigned int i = 0; i < elemCount; ++i)
	{
		const BRZ::RigidElement & src = elements[i];

		if (src.Empty() || !src.active) 
			continue;

		float testRadius = (src.local.position.Length() + src.radius);

		if (testRadius > radius)
			radius = testRadius;
	}

	bounds = radius + 5.0f;

	return BRZ_SUCCESS;
}


//	Note: Constructs the rigid elements that the RigidObject in question possesses,
//			based on the asset being constructed from.  Note that the RigidObject is
//			not in a usable state until this method has been invoked.
//		  Observe that typically this method is invoked by the Actor owning the RigidObject
//			during its own construction from the asset in question.
BRZRESULT BRZ::RigidObject::Construct(const BRZ::Asset & A_asset)
{
	// Ensure the RigidObject is un-constructed internally:
	if (elements != NULL)
		return BRZ_FAILURE;

	elemCount = A_asset.NumElements();

	if (elemCount == 0)
		return BRZ_SUCCESS;

	// Construct the individual elements by extraction from the asset:
	elements = new BRZ::RigidElement[elemCount];
	for (unsigned int i = 0; i < elemCount; ++i)
	{
		if (A_asset.ExtractCollision(elements[i], i) != BRZ_SUCCESS)
			return BRZ_FAILURE;
	}

	this->RecalculateBounds();	

	return BRZ_SUCCESS;
}


BRZ::RigidObject::RigidObject(const BRZ::Orientation & A_world) :
	world(A_world),
	elements(NULL),
	elemCount(0),
	radius(0),
	bounds(0)
{
}


BRZ::RigidObject::~RigidObject()
{
	if (elements != NULL)
		delete [] elements;
}