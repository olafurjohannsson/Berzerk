
#include "BRZ_LineElement.h"


BRZ::LineElement::LineElement() :
	ptOffset(0),
	ptCount(0),
	idxOffset(0),
	idxCount(0)
{
}


BRZ::LineElement::LineElement(const BRZ::LineElement & A_ref) :
	colour(A_ref.colour),
	ptOffset(A_ref.ptOffset),
	ptCount(A_ref.ptCount),
	idxOffset(A_ref.idxOffset),
	idxCount(A_ref.idxCount),
	localPos(A_ref.localPos)
{
}


BRZ::LineElement::~LineElement()
{
}