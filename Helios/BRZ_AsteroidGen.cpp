
#include "BRZ_AsteroidGen.h"

#include "BRZ_Colour.h"
#include "BRZ_Coord2.h"
#include "BRZ_RawGeometry.h"
#include "BRZ_RawElement.h"
#include "BRZ_Vec2.h"

BRZRESULT BRZ::AsteroidGen::MakeRock(BRZ::RawGeometry & A_out)
{
	if (A_out.count != 0)
		return BRZ_FAILURE;

	unsigned int				pointCount = 12;
	float						radius = 40.0f;
	float						theta = 0.0f;
	float						dTheta = (BRZ::TWO_PI / pointCount);
	std::vector<BRZ::Coord2>	lines;
	std::vector<BRZ::Vec2>		points;

	A_out.count = 1;
	A_out.elem = new BRZ::RawElement[1];

	for (unsigned int i = 0; i < pointCount; ++i)
	{
		float radMod = (float)((rand() % 1000) - 500);
		radMod = radMod / 200.0f;

		BRZ::Vec2 pos(cos(theta + (i * dTheta)) * (radius + radMod), sin(theta + (i * dTheta)) * (radius + radMod));
		points.push_back(pos);

		if (i > 0)
			lines.push_back(BRZ::Coord2(i, i - 1));
		if (i == pointCount - 1)
			lines.push_back(BRZ::Coord2(i, 0));
	}

	unsigned int index = 0;
	A_out.elem[0].AllocLines(lines.size());
	for (auto i = lines.cbegin(); i != lines.cend(); ++i)
	{
		A_out.elem[0].lines[index] = *i;
		++index;
	}

	index = 0;
	A_out.elem[0].AllocPoints(points.size());
	for (auto i = points.cbegin(); i != points.cend(); ++i)
	{
		A_out.elem[0].points[index] = *i;
		++index;
	}

	A_out.elem[0].colour = BRZ::Colour(1.0f, 1.0f, 0.0f, 1.0f);
	return BRZ_SUCCESS;
}