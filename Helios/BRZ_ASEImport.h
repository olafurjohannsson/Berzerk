
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_ASEImport.h" )
#endif

#include "BRZ_Define.h"

#include "BRZ_Colour.h"
#include "BRZ_Coord2.h"
#include "BRZ_Vec2.h"

namespace BRZ
{
	class ASEImport
	{
	public:
		struct Element;

	public:
		ASEImport(std::ofstream & log);
		~ASEImport();

	public:
		BRZRESULT		Import(const BRZSTRING & file);

		// Acquire the pure geometry as a RawGeometry Object:
		BRZRESULT		CreateGeometry(BRZ::RawGeometry & output);

		// Acquire access to the elements themselves:
		unsigned int		NumElements() const;
		const Element *		AcquireElement(unsigned int index);

	protected:
		BRZRESULT	OpenNewElement();
		BRZRESULT	ParseElement();
		BRZRESULT	CloseElement();
		

		BRZRESULT	ParseNextCommand();

		BRZRESULT	ResetImporter();
		BRZRESULT	Note(const std::string & message);

	private:
		std::ofstream &			log;
		std::wifstream			file;
		std::wstring			line;
		unsigned int			lineID;
		std::wstring			command;
		std::wistringstream		tokens;
		
		Element *				active;
		unsigned int			indent;
		unsigned int			max_sets;
		unsigned int			now_sets;
		unsigned int			max_pts;
		unsigned int			now_pts;
		unsigned int			pt_idx;
		bool					closed;
		std::vector<Element *>	elem;


	public:
		struct Element
		{
		public:
			Element() : name(L""), rigid(false)	{    }

		public:
			BRZSTRING					name;
			BRZ::Colour					colour;
			bool						rigid;
			std::vector<BRZ::Vec2>		points;
			std::vector<BRZ::Coord2>	lines;
		};
	};
}