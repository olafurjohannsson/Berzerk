
#include "BRZ_LineObject.h"

#include "BRZ_Display.h"
#include "BRZ_LineElement.h"

using namespace DirectX;

BRZRESULT BRZ::LineObject::Render(const BRZ::Vec2 & A_pos, float A_rot)
{
	if (display == NULL)
		return BRZ_FAILURE;

	for (unsigned int i = 0; i < numElements; ++i)
	{
		XMFLOAT4X4		transform;

		if (elements[i].standard)
		{
			XMStoreFloat4x4(&transform, XMMatrixIdentity() * XMMatrixRotationZ(A_rot) * XMMatrixTranslation(A_pos.x + elements[i].base->localPos.x, A_pos.y + elements[i].base->localPos.y, 0));
			display->Queue(*elements[i].base, transform, elements[i].base->colour);
		}
		else
		{
			XMStoreFloat4x4(&transform, XMMatrixIdentity() * XMMatrixRotationZ(A_rot + elements[i].rotation) * XMMatrixTranslation(A_pos.x + elements[i].offset.x, A_pos.y + elements[i].offset.y, 0));
			display->Queue(*elements[i].base, transform, elements[i].colour);
		}
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