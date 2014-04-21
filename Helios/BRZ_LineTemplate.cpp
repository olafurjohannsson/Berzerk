
#include "BRZ_LineTemplate.h"


void BRZ::LineTemplate::AllocElems(unsigned int A_num)
{
	if (idElements != NULL)
		delete [] idElements;

	if (A_num != 0)
	{
		idElements = new unsigned int[A_num];
		BRZ::ZeroMem(idElements, A_num);
		numElements = A_num;
	}
	else
	{
		idElements = NULL;
		numElements = 0;
	}
}


BRZ::LineTemplate::LineTemplate() : 
	numElements(0),
	idElements(NULL),
	name(L"")
{
}


BRZ::LineTemplate::LineTemplate(const BRZ::LineTemplate & A_ref) :
	numElements(0),
	idElements(NULL),
	name(A_ref.name)
{
	this->AllocElems(A_ref.numElements);
	BRZ::CopyMem(idElements, A_ref.idElements, A_ref.numElements);
}


BRZ::LineTemplate::~LineTemplate()
{
	if (idElements != NULL)
		delete [] idElements;
}