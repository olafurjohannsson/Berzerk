
#include "BRZ_RawGeometry.h"

#include "BRZ_Coord2.h"
#include "BRZ_RawElement.h"
#include "BRZ_Vec2.h"



BRZ::RawGeometry::RawGeometry(const BRZ::RawGeometry & A_ref) :
	count(A_ref.count)
{
	elem = new BRZ::RawElement[count];

	for (unsigned int i = 0; i < count; ++i)
	{
		elem[i].colour = A_ref.elem[i].colour;
		elem[i].AllocLines(A_ref.elem[i].lineCount);
		for (unsigned int j = 0; j < elem[i].lineCount; ++j)
		{
			elem[i].lines[j] = A_ref.elem[i].lines[j];
		}

		elem[i].AllocPoints(A_ref.elem[i].pointCount);
		for (unsigned int j = 0; j < elem[i].pointCount; ++j)
		{
			elem[i].points[j] = A_ref.elem[i].points[j];
		}

		elem[i].offset = A_ref.elem[i].offset;
	}
}


BRZ::RawGeometry::RawGeometry() :
	elem(NULL),
	count(0)
{
}


BRZ::RawGeometry::~RawGeometry()
{
	if (elem != NULL)
		delete [] elem;
}