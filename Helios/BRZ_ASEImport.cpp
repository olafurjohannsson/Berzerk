
#include "BRZ_ASEImport.h"

#include "BRZ_Coord2.h"
#include "BRZ_RawElement.h"
#include "BRZ_RawGeometry.h"
#include "BRZ_Vec2.h"


unsigned int BRZ::ASEImport::NumElements() const
{
	return elem.size();
}


const BRZ::ASEImport::Element * BRZ::ASEImport::AcquireElement(unsigned int A_idx)
{
	if (A_idx >= elem.size())
		return NULL;

	return elem.at(A_idx);
}


BRZRESULT BRZ::ASEImport::CreateGeometry(BRZ::RawGeometry & A_geo)
{
	// Will only operate on non-filled rawGeometry objects:
	if (A_geo.count != 0)
		return BRZ_FAILURE;

	A_geo.count = elem.size();
	A_geo.elem = new BRZ::RawElement[A_geo.count];

	unsigned int index = 0;
	for (auto i = elem.cbegin(); i != elem.cend(); ++i)
	{
		unsigned int				local = 0;
		BRZ::ASEImport::Element &	src = *(*i);
		BRZ::RawElement &			obj = A_geo.elem[index];

		// obj.colour = BRZ::Colour(1.0f, 0.0f, 0.0f, 1.0f);
		obj.colour = src.colour;
		obj.AllocPoints(src.points.size());
		for (auto j = src.points.cbegin(); j != src.points.cend(); ++j)
		{
			obj.points[local] = *j;
			local++;
		}

		local = 0;
		obj.AllocLines(src.lines.size());
		for (auto j = src.lines.cbegin(); j != src.lines.cend(); ++j)
		{
			obj.lines[local] = *j;
			local++;
		}

		++index;
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::Note(const std::string & A_msg)
{
	std::string prefix = "[ASEImport]: ";
	
	std::string body = prefix + A_msg;
	log << body << std::endl;
	log.flush();

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::Import(const BRZSTRING & A_file)
{
	this->ResetImporter();

	file.open(BRZ::Narrow(A_file).c_str());

	if (!file.is_open())
	{
		BRZSTRING message = L"Failed to open ASE file: ";
		message = message + A_file;
		Note(BRZ::Narrow(message));
		return BRZ_FAILURE;
	}

	while(!file.eof())
	{
		if (this->ParseNextCommand() != BRZ_SUCCESS)
		{
			BRZSTRING head = L"Error in ASE file: ";
			head = head + A_file;
			Note(BRZ::Narrow(head));

			std::wostringstream body;
			body << L"Command parser error in line [" << lineID << L"]: " << line;
			Note(BRZ::Narrow(body.str()));

			this->ResetImporter();
			return BRZ_FAILURE;
		}

		if (command == L"SHAPEOBJECT")
		{
			if (this->ParseElement() != BRZ_SUCCESS)
			{
				Note("Failed to parse element.");
				return BRZ_FAILURE;
			}
		}
	}

	file.close();
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::ParseElement()
{
	// The initial indentation for a SHAPEOBJECT declaration is 1;
	//	when indent == 0 the object is finished.
	if (indent != 1)
	{
		Note("Invalid indentation for element parsing.");
		this->ResetImporter();
		return BRZ_FAILURE;
	}

	this->OpenNewElement();

	while (indent != 0)
	{
		this->ParseNextCommand();


		if (command == L"SHAPE_LINECOUNT")
		{
			tokens >> max_sets;
			continue;
		}
		else if (command == L"SHAPE_LINE")
		{
			closed = false;
			max_pts = 0;
			now_pts = 0;

			tokens >> now_sets;
			if (tokens.fail())
				Note("failed to read line shape index.");

			if (now_sets >= max_sets)
				return BRZ_FAILURE;

			continue;
		}
		else if (command == L"SHAPE_CLOSED")
		{
			closed = true;
			continue;
		}
		else if (command == L"SHAPE_VERTEXCOUNT")
		{
			tokens >> max_pts;
			continue;
		}
		else if (command == L"SHAPE_VERTEX_KNOT")
		{
			tokens >> now_pts;

			// Check for invalid point index:
			if (now_pts >= max_pts)
				return BRZ_FAILURE;

			BRZ::Vec2	pt;
			tokens >> pt.x;
			tokens >> pt.y;
			active->points.push_back(pt);

			if (now_pts > 0)
			{
				BRZ::Coord2		face(pt_idx - 1, pt_idx);
				active->lines.push_back(face);
			}

			if (now_pts == (max_pts - 1) && closed)
			{
				BRZ::Coord2		face(pt_idx, pt_idx - (max_pts - 1));
				active->lines.push_back(face);
			}

			pt_idx++;
			continue;
		}
		else if (command == L"HELIOS_COLLIDES")
		{
			active->rigid = true;
			continue;
		}
		else if (command == L"HELIOS_COLOUR")
		{
			tokens >> active->colour.r;
			tokens >> active->colour.g;
			tokens >> active->colour.b;
			tokens >> active->colour.a;

			continue;
		}
	}

	this->CloseElement();

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::ParseNextCommand()
{
	line = L"";
	lineID++;
	command = L"";
	tokens.str(L"");
	tokens.clear();

	BRZSTRING	whitespace = L"\t\n ";
	BRZSTRING	comSymbol = L"*";
	BRZSTRING	delimIn = L"{";
	BRZSTRING	delimOut = L"}";


	std::getline(file, line);
	if (line.empty())
		return BRZ_SUCCESS;

	BRZ::TrimString(line);

	// Update our current indentation.
	if (line.find_first_of(delimIn) != BRZSTRING::npos)
		indent++;
	else if (line.find_first_of(delimOut) != BRZSTRING::npos)
		indent--;

	// Verify that the line contains a command (will start with comSymbol):
	if (line.substr(0, 1) != comSymbol)
		return BRZ_SUCCESS;
	else
		line = line.substr(1, line.length() - 1);

	// Split the line into commands and arguments:
	BRZSTRING::size_type	splitIDX = line.find_first_of(whitespace);
	BRZSTRING				argStr = line.substr(splitIDX + 1, line.length() - (splitIDX + 1));
	command = line.substr(0, splitIDX);
	tokens.str(argStr);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::CloseElement()
{
	if (active == NULL)
		return BRZ_FAILURE;

	max_sets = 0;
	now_sets = 0;
	max_pts = 0;
	now_pts = 0;
	pt_idx = 0;
	closed = false;
	elem.push_back(active);
	active = NULL;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::OpenNewElement()
{
	if (active != NULL)
		return BRZ_FAILURE;

	active = new BRZ::ASEImport::Element;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::ASEImport::ResetImporter()
{
	if (active != NULL)
		delete active;

	indent = 0;
	max_sets = 0;
	now_sets = 0;
	max_pts = 0;
	now_pts = 0;
	pt_idx = 0;
	closed = false;

	if (file.is_open())
		file.close();

	line = L"";
	lineID = 0;
	command = L"";
	tokens.str(L"");

	for (auto i = elem.begin(); i != elem.end(); ++i)
		delete *i;
	elem.clear();

	return BRZ_SUCCESS;
}


BRZ::ASEImport::ASEImport(std::ofstream & A_log) :
	log(A_log),
	line(L""),
	lineID(0),
	command(L""),
	active(NULL),
	indent(0),
	max_sets(0),
	now_sets(0),
	max_pts(0),
	now_pts(0),
	pt_idx(0),
	closed(false)
{
}


BRZ::ASEImport::~ASEImport()
{
	this->ResetImporter();
}