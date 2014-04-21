
#include "BRZ_RawElement.h"

#include "BRZ_Coord2.h"
#include "BRZ_Vec2.h"


void BRZ::RawElement::AllocLines(unsigned int A_num)
{
	if (lines != NULL)
	{
		delete [] lines;
		lines = NULL;
	}

	if (A_num > 0)
		lines = new BRZ::Coord2[A_num];

	lineCount = A_num;
}


void BRZ::RawElement::AllocPoints(unsigned int A_num)
{
	if (points != NULL)
	{
		delete [] points;
		points = NULL;
	}

	if (A_num > 0)
		points = new BRZ::Vec2[A_num];

	pointCount = A_num;
}


BRZ::RawElement::RawElement(const BRZ::RawElement & A_ref) :
	colour(A_ref.colour),
	offset(A_ref.offset),
	pointCount(A_ref.pointCount),
	lineCount(A_ref.lineCount)
{
	if (pointCount > 0)
	{
		points = new BRZ::Vec2[pointCount];
		BRZ::CopyMem(points, A_ref.points, pointCount);
	}

	if (lineCount > 0)
	{
		lines = new BRZ::Coord2[lineCount];
		BRZ::CopyMem(lines, A_ref.lines, lineCount);
	}
}


BRZ::RawElement::RawElement() :
	points(NULL),
	lines(NULL),
	pointCount(0),
	lineCount(0)
{
}


BRZ::RawElement::~RawElement()
{
	if (points != NULL)
		delete [] points;

	if (lines != NULL)
		delete [] lines;
}