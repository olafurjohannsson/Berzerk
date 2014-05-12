
#include "BRZ_AsteroidGen.h"

#include "BRZ_Asset.h"
#include "BRZ_Config.h"
#include "BRZ_RawGeometry.h"
#include "BRZ_RawElement.h"


unsigned int BRZ::AsteroidGen::counter = 0;


BRZRESULT BRZ::AsteroidGen::SubDivide(const BRZ::Vec2 & A_ptA, float A_radA, const BRZ::Vec2 & A_ptB, float A_radB, \
									  std::vector<BRZ::Vec2> & A_ptStore)
{
	// Increment the recursive fractal depth:
	fractalDepth++;

	float			range = cfg.ReadFloat(L"FractalBaseRange");
	float			decay = pow(cfg.ReadFloat(L"FractalDecayRate"), (fractalDepth - 1));
	unsigned int	maxDepth = cfg.ReadInt(L"FractalSubdivisions");

	float		newLength = (A_ptA.Length() + A_ptB.Length()) * 0.5f;
	float		modLength = 1 + BRZ::RandomFloat((range * decay * -1), (range * decay));
	float		newRad = (A_radA + A_radB) * 0.5f;

	BRZ::Vec2	newVec = A_ptA.Rotate(newRad - A_radA).Normalize();

	newVec = newVec * (modLength * newLength);

	if (fractalDepth < maxDepth)
		this->SubDivide(A_ptA, A_radA, newVec, newRad, A_ptStore);

	A_ptStore.push_back(newVec);

	if (fractalDepth < maxDepth)
		this->SubDivide(newVec, newRad, A_ptB, A_radB, A_ptStore);

	--fractalDepth;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::AsteroidGen::MakeFractalRock(BRZ::Asset & A_out, float A_radius, unsigned int A_surfaces)
{
	if (A_surfaces == 0)
		return BRZ_FAILURE;

	BRZ::Vec2		baseVec = BRZ::Vec2(0, 1) * A_radius;
	float			baseRad = 0;
	float			surfaceRad = BRZ::TWO_PI / static_cast<float>(A_surfaces);

	unsigned int	subDivs = cfg.ReadInt(L"FractalSubdivisions");

	if (subDivs <= 0)
		return BRZ_FAILURE;

	std::vector<BRZ::Coord2>	lines;
	std::vector<BRZ::Vec2>		points;

	unsigned int				baseIndex = 0;
	unsigned int				maxIndex = 0;

	// Split the circle down into surfaces:
	for (unsigned int i = 0; i < A_surfaces; ++i)
	{
		baseIndex = points.size();

		float		startRad	= static_cast<float>(i) * surfaceRad;
		BRZ::Vec2	startVec	= baseVec.Rotate(startRad);

		float		endRad		= static_cast<float>(i + 1) * surfaceRad;
		BRZ::Vec2	endVec		= baseVec.Rotate(endRad);

		points.push_back(startVec);
		this->SubDivide(startVec, startRad, endVec, endRad, points);
		points.push_back(endVec);

		maxIndex = (points.size() - 1) - baseIndex;

		// Generate the lines connecting the points:
		for (unsigned int j = 0; j < maxIndex; ++j)
		{
			BRZ::Coord2		newLine((j + baseIndex), (j + baseIndex) + 1);
			lines.push_back(newLine);
		}
	}

	// Prepare a RawElement to be imported by the Asset object for usage:
	BRZ::RawElement		outElem;

	outElem.colour = BRZ::Colour(0.0f, 0.7f, 0.0f, 1.0f);
	outElem.offset = BRZ::Vec2(0, 0);
	outElem.AllocPoints(points.size());

	unsigned int index = 0;
	for (auto i = points.cbegin(); i != points.cend(); ++i)
	{
		outElem.points[index] = *i;
		++index;
	}

	outElem.AllocLines(lines.size());

	index = 0;
	for (auto i = lines.cbegin(); i != lines.cend(); ++i)
	{
		outElem.lines[index] = *i;
		++index;
	}

	// Produce a unique name for the asteroid

	BRZSTRING	prefix = L"FRAC_ROCK_";
	BRZOSTREAM	name;

	name << prefix << counter;
	++counter;

	return A_out.ImportProceduralObject(outElem, name.str(), true);
}




BRZRESULT BRZ::AsteroidGen::MakeGenericRock(BRZ::RawGeometry & A_out)
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