
#include "BRZ_LineObject.h"

#include "BRZ_Display.h"
#include "BRZ_LineElement.h"

using namespace DirectX;


BRZRESULT BRZ::LineObject::RotateElement(unsigned int A_idx, const float & A_rot)
{
	if (A_idx >= numElements)
		return BRZ_FAILURE;

	float realRot = BRZ::ClampRotation(A_rot);

	elements[A_idx].rotation = realRot;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::LineObject::PlaceElement(unsigned int A_idx, const BRZ::Vec2 & A_pos)
{
	if (A_idx >= numElements)
		return BRZ_FAILURE;

	elements[A_idx].offset = A_pos;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::LineObject::HideElement(unsigned int A_idx)
{
	if (A_idx >= numElements)
		return BRZ_FAILURE;

	elements[A_idx].visible = false;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::LineObject::ShowElement(unsigned int A_idx)
{
	if (A_idx >= numElements)
		return BRZ_FAILURE;

	elements[A_idx].visible = true;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::LineObject::Render(const BRZ::Vec2 & A_pos, float A_rot)
{
	if (display == NULL)
		return BRZ_FAILURE;

	for (unsigned int i = 0; i < numElements; ++i)
	{
		XMFLOAT4X4		transform;

		if (!elements[i].visible)
			continue;

		// Translate the local position of the element with respect to the rotation of
		//	the whole object -- local position is defined as relative to the object centre
		//	where rotation = 0; thus when the object is rotated the local position of 
		//	any element not located on the origin changes.

		BRZ::Vec2 newPos = elements[i].offset.Rotate(A_rot);

		/*
		BRZ::Vec2	oldPos(elements[i].offset);
		BRZ::Vec2	newPos(0, 0);
		BRZ::Vec2	xContribution(cos(A_rot), sin(A_rot));
		xContribution = xContribution * oldPos.x;

		BRZ::Vec2	yContribution(cos(BRZ::ClampRotation(A_rot + BRZ::HALF_PI)), sin(BRZ::ClampRotation(A_rot + BRZ::HALF_PI)));
		yContribution = yContribution * oldPos.y;

		newPos = xContribution + yContribution;
		*/



		XMStoreFloat4x4(&transform, XMMatrixIdentity() * XMMatrixRotationZ(A_rot + elements[i].rotation) * XMMatrixTranslation(A_pos.x + newPos.x, A_pos.y + newPos.y, 0));
		display->Queue(*elements[i].base, transform, elements[i].colour);
	}

	return BRZ_SUCCESS;
}


BRZ::LineObject::LineObject() :
	display(NULL),
	elements(NULL),
	numElements(0)
{
}


BRZ::LineObject::~LineObject()
{
	if (elements != NULL)
		delete [] elements;
}