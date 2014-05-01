
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Config.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Config
	{
	public:
		Config(std::ofstream & log);
		~Config();

	public:
		BRZRESULT	LoadFile(const BRZSTRING & cfgFile);
		
		const BRZSTRING &		ReadString(const BRZSTRING & variable);
		const signed int &		ReadInt(const BRZSTRING & variable);
		const float &			ReadFloat(const BRZSTRING & variable);

	protected:
		BRZRESULT	ParseLine(const BRZSTRING & line);
		void		Note(const BRZSTRING & msg);
		bool		VarExists(const BRZSTRING & name);

	private:
		std::ofstream &						log;
		bool								loaded;

		std::map<BRZSTRING, BRZSTRING>		strVars;
		std::map<BRZSTRING, signed int>		intVars;
		std::map<BRZSTRING, float>			fltVars;

	private:
		// References returned when attempting to read a variable that does not exist:
		static const BRZSTRING		BAD_STRING;
		static const signed int		BAD_INT;
		static const float			BAD_FLOAT;
	};
}