
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_ASEImport.h" )
#endif

#include "BRZ_Define.h"

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
		BRZRESULT Import(const BRZSTRING & file, BRZ::RawGeometry & output);

	protected:
		BRZRESULT	OpenNewElement();
		BRZRESULT	ParseElement();
		BRZRESULT	CloseElement();
		BRZRESULT	CreateGeometry(BRZ::RawGeometry & output);

		BRZRESULT	ParseNextCommand();
		void		TrimLine();

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
			BRZSTRING					name;
			std::vector<BRZ::Vec2>		points;
			std::vector<BRZ::Coord2>	lines;
		};
	};
}