
#include "BRZ_Asset.h"

#include "BRZ_ASEImport.h"
#include "BRZ_DiskStream.h"
#include "BRZ_RawElement.h"
#include "BRZ_RawGeometry.h"
#include "BRZ_RigidObject.h"




//	Note: Produces a RigidElement (usable by the RigidObject class for collision detection)
//			from the component information specified by the index.
//		  Observe that this function is typically invoked by a RigidObject in the process of
//			its "Construct(...)" method.
BRZRESULT BRZ::Asset::ExtractCollision(BRZ::RigidElement & A_out, unsigned int A_idx) const
{
	// Ensure the validity of the index:
	if (A_idx >= subCount)
		return BRZ_FAILURE;

	const BRZ::Asset::Component & src = subObjects[A_idx];

	if (!src.isRigid || src.lines.size() == 0)
	{
		// If the component in question is not rigid (does not collide) or if the component is
		//	an empty component, leave the RigidElement empty, merely updating its local position.
		A_out.local.position = src.localOrigin;
		return BRZ_SUCCESS;
	}


	A_out.lineCount = src.lines.size();
	A_out.lines = new BRZ::RigidLine[A_out.lineCount];

	// Copy over the line information in the form of RigidLines (each containing their own
	//	point information, rather than referencing a point vector):
	for (unsigned int i = 0; i < A_out.lineCount; ++i)
	{
		BRZ::RigidLine &	line = A_out.lines[i];

		unsigned int a = src.lines.at(i).x;
		unsigned int b = src.lines.at(i).y;

		line.point[0] = src.points.at(a);
		line.point[1] = src.points.at(b);
	}

	// Extract the radii, and the origin:
	A_out.local.position = src.localOrigin;
	A_out.radius = src.radius;
	A_out.boundRadius = src.boundRadius;

	return BRZ_SUCCESS;
}


//	Note: Produces a RawElement from the component information specified
//			by the index;
//		  Observe that this function is typically invoked in the process of
//			extracting RawGeometry.
BRZRESULT BRZ::Asset::ExtractElement(BRZ::RawElement & A_out, unsigned int A_idx) const
{
	// Ensure the validity of the index:
	if (A_idx >= subCount)
		return BRZ_FAILURE;

	const BRZ::Asset::Component &	src = subObjects[A_idx];

	A_out.colour = src.colour;
	A_out.offset = src.localOrigin;

	// Copy line data:
	if (!src.lines.empty())
	{
		A_out.AllocLines(src.lines.size());

		unsigned int index = 0;
		for (auto i = src.lines.cbegin(); i != src.lines.cend(); ++i)
		{
			A_out.lines[index] = *i;
			++index;
		}
	}

	// Copy point data:
	if (!src.points.empty())
	{
		A_out.AllocPoints(src.points.size());

		unsigned int index = 0;
		for (auto i = src.points.cbegin(); i != src.points.cend(); ++i)
		{
			A_out.points[index] = *i;
			++index;
		}
	}

	return BRZ_SUCCESS;
}


//	Note: Produces a RawGeometry object fit for usage by the BRZ::Display from
//			the internally loaded asset information.
BRZRESULT BRZ::Asset::ExtractGeometry(BRZ::RawGeometry & A_out) const
{
	// Will not extract to an object already containing geometry.
	if (A_out.count != 0)
		return BRZ_FAILURE;

	// If the asset contains no geometry.. do nothing.
	if (subCount == 0)
		return BRZ_SUCCESS;

	A_out.count = subCount;
	A_out.elem = new BRZ::RawElement[subCount];

	for (unsigned int i = 0; i < subCount; ++i)
		this->ExtractElement(A_out.elem[i], i);

	return BRZ_SUCCESS;
}


unsigned int BRZ::Asset::NumElements() const
{
	return subCount;
}


const BRZSTRING & BRZ::Asset::Name() const
{
	return name;
}


BRZ::Vec2 BRZ::Asset::ElementPosition(unsigned int A_idx) const
{
	if (A_idx >= subCount)
		return BRZ::Vec2(0, 0);

	return subObjects[A_idx].localOrigin;
}


//	Note: Processess a component that has been loaded from an ASE file to prepare it
//			for usage within Helios.
BRZRESULT BRZ::Asset::ProcessComponent(BRZ::Asset::Component & A_src)
{
	// Find the "mid point" of the point set, update the origin and fix the placement
	//	of all points to reflect their new origin p(x, y) -> p(x - origin.x, y - origin.y)
	BRZ::Vec2	avg(0, 0);
	float		count = 0;

	for (auto i = A_src.points.cbegin(); i != A_src.points.cend(); ++i)
	{
		avg = avg + *i;
		count = count + 1.0f;
	}

	avg = avg * (1 / count);
	A_src.localOrigin = avg;

	for (auto i = A_src.points.begin(); i != A_src.points.end(); ++i)
		*i = *i - avg;


	// Now with the fixed origin and element placement, determine the radius (furthest
	//	point from origin) and the bounding radius (radius + 5):
	float	maxRad = 0;
	for (auto i = A_src.points.cbegin(); i != A_src.points.cend(); ++i)
	{
		if (i->SqrLength() > maxRad)
			maxRad = i->SqrLength();
	}
	A_src.radius = sqrt(maxRad);
	A_src.boundRadius = A_src.radius + 5.0f;

	return BRZ_SUCCESS;
}


//	Note: Function imported a procedurally generated RawElement as an asset.  Currently procedural
//			imports are limited to a single element, although this may change in future builds.
//		  Observe that this function is invoked by the AsteroidGen class.
BRZRESULT BRZ::Asset::ImportProceduralObject(const BRZ::RawElement & A_geo, const BRZSTRING & A_name, bool A_rigid)
{
	if (loaded)
		return BRZ_FAILURE;

	name = A_name;
	subCount = 1;
	subObjects = new BRZ::Asset::Component[subCount];

	BRZ::Asset::Component &			target = subObjects[0];

	target.colour = A_geo.colour;
	target.isRigid = A_rigid;

	// Copy point data:
	target.points.reserve(A_geo.pointCount);
	for (unsigned int i = 0; i < A_geo.pointCount; ++i)
		target.points.push_back(A_geo.points[i]);

	// Copy line data:
	target.lines.reserve(A_geo.lineCount);
	for (unsigned int i = 0; i != A_geo.lineCount; ++i)
		target.lines.push_back(A_geo.lines[i]);

	this->ProcessComponent(target);
	// Note that the element should have its origin at (0, 0):
	target.localOrigin = BRZ::Vec2(0, 0);

	loaded = true;
	return BRZ_SUCCESS;
}


//	Note: Exports internally loaded asset to a helios asset file, extension: .haf
BRZRESULT BRZ::Asset::ExportHeliosAsset(const BRZSTRING & A_file)
{
	if (!loaded)
		return BRZ_FAILURE;

	// Write the file as so:
	//	[int]			number of Components
	//	[Component]		component[0]
	//	[Component]		component[1]
	//		....
	//	[Component]		component[n]

	BRZ::DiskStream	output;
	output.OpenWrite(A_file);

	signed int outCount = subCount;
	output.Pack(outCount);

	for (unsigned int i = 0; i < subCount; ++i)
	{
		output.Pack(subObjects[i]);
	}

	output.Close();

	return BRZ_SUCCESS;
}

//	Note: Imports a helios asset file, extension: .haf, into its internal
//			representation.
BRZRESULT BRZ::Asset::ImportHeliosAsset(const BRZSTRING & A_file)
{
	if (loaded)
		return BRZ_FAILURE;

	// See file format description in ::ExportHeliosAsset(...).


	// Determine the name from the file-name:
	BRZSTRING				dir = L"\\/";
	BRZSTRING				ext = L".";
	BRZSTRING::size_type	begin = A_file.find_last_of(dir);

	if (begin == BRZSTRING::npos)
		begin = 0;
	else
		begin = begin + 1;

	BRZSTRING::size_type	end = A_file.find_first_of(ext);
	name = A_file.substr(begin, (end - begin));

	BRZ::DiskStream		input;
	input.OpenRead(A_file);

	signed int	inCount = 0;
	input.Unpack(inCount);

	if (inCount == 0)
	{
		loaded = true;
		input.Close();
		return BRZ_SUCCESS;
	}

	subCount = inCount;
	subObjects = new BRZ::Asset::Component[subCount];

	for (unsigned int i = 0; i < subCount; ++i)
		input.Unpack(subObjects[i]);

	loaded = true;
	input.Close();
	return BRZ_SUCCESS;
}

//	Note: Funtion imports a geometry file in .ASE format to a Helios Asset
//			stored internally.  This is done by invoking the ASEImporter class
//			to parse the ASE file itself into its raw representation of the geo
//			data.
//		  Subsequently, the Elements determined by the ASEImporter are translated
//			into the asset component format, and processed to update some of their
//			characteristics to prepare them for usage in-game.
BRZRESULT BRZ::Asset::ImportASEObject(const BRZSTRING & A_file, std::ofstream & A_log)
{
	if (loaded)
		return BRZ_FAILURE;

	BRZ::ASEImport	importer(A_log);

	if (importer.Import(A_file) != BRZ_SUCCESS)
		return BRZ_FAILURE;

	// Determine the name from the file-name:
	BRZSTRING				dir = L"\\/";
	BRZSTRING				ext = L".";
	BRZSTRING::size_type	begin = A_file.find_last_of(dir);

	if (begin == BRZSTRING::npos)
		begin = 0;
	else
		begin = begin + 1;

	BRZSTRING::size_type	end = A_file.find_last_of(ext);
	name = A_file.substr(begin, (end - begin));

	if (importer.NumElements() == 0)
	{
		loaded = true;
		return BRZ_SUCCESS;
	}

	subCount = importer.NumElements();
	subObjects = new BRZ::Asset::Component[subCount];

	for (unsigned int i = 0; i < subCount; ++i)
	{
		BRZ::Asset::Component &			target = subObjects[i];
		const BRZ::ASEImport::Element &	source = *importer.AcquireElement(i);

		target.colour = source.colour;
		target.isRigid = source.rigid;

		// Copy point data:
		target.points.reserve(source.points.size());
		for (auto i = source.points.cbegin(); i != source.points.cend(); i++)
			target.points.push_back(*i);

		// Copy line data:
		target.lines.reserve(source.lines.size());
		for (auto i = source.lines.cbegin(); i != source.lines.cend(); i++)
			target.lines.push_back(*i);

		this->ProcessComponent(target);
	}

	loaded = true;
	return BRZ_SUCCESS;
}


BRZ::Asset::Asset() :
	loaded(false),
	name(L""),
	subObjects(NULL),
	subCount(0)
{
}



BRZ::Asset::~Asset()
{
	if (subObjects != NULL)
		delete [] subObjects;
}




BRZRESULT BRZ::Asset::Component::Unpack(BRZ::DiskStream & A_in)
{
	A_in.Unpack(localOrigin);
	A_in.Unpack(radius);
	A_in.Unpack(boundRadius);
	A_in.Unpack(colour);
	A_in.Unpack(isRigid);

	signed int	numPoints = 0;
	A_in.Unpack(numPoints);
	if (numPoints > 0)
	{
		points.reserve(numPoints);
		for (signed int i = 0; i < numPoints; ++i)
		{
			BRZ::Vec2	pt;
			A_in.Unpack(pt);
			points.push_back(pt);
		}
	}

	signed int	numLines = 0;
	A_in.Unpack(numLines);
	if (numLines > 0)
	{
		lines.reserve(numLines);
		for (signed int i = 0; i < numLines; ++i)
		{
			BRZ::Coord2 line;
			A_in.Unpack(line);
			lines.push_back(line);
		}
	}

	return BRZ_SUCCESS;
}


unsigned int BRZ::Asset::Component::Pack(BRZ::DiskStream & A_out) const
{
	A_out.Pack(localOrigin);
	A_out.Pack(radius);
	A_out.Pack(boundRadius);
	A_out.Pack(colour);
	A_out.Pack(isRigid);

	signed int	numPoints = points.size();
	A_out.Pack(numPoints);
	if (numPoints > 0)
	{
		for (auto i = points.cbegin(); i != points.cend(); ++i)
		{
			A_out.Pack(*i);
		}
	}

	signed int	numLines = lines.size();
	A_out.Pack(numLines);
	if (numLines > 0)
	{
		for (auto i = lines.cbegin(); i != lines.cend(); ++i)
		{
			A_out.Pack(*i);
		}
	}

	unsigned int sizeDelta = sizeof(localOrigin) + sizeof(radius) + sizeof(boundRadius) + sizeof(colour) + \
		sizeof(isRigid) + sizeof(numPoints) + (numPoints * sizeof(BRZ::Vec2)) + sizeof(numLines) + \
		(numLines * sizeof(BRZ::Coord2));

	return sizeDelta;
}


BRZ::Asset::Component::Component() :
	radius(0),
	boundRadius(0),
	isRigid(false)
{
}


BRZ::Asset::Component::~Component()
{
}