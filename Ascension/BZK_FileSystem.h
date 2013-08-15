#include "BZK_Typedef.h"
#include "BZK_LoggerService.h"
#include <string>

#pragma once

#ifdef BZK_HEADERTRACE
#pragma message "Header trace: BZK_FileSystem.h"
#endif


using namespace std;

namespace BZK
{
	class FileSystem
	{
	public:
		FileSystem();
		FileSystem(const BZKSTRING &p) : path(p) 
		{
			this->path = p;
		};


		~FileSystem();

		
		void CreateDir(const BZKSTRING &name, bool is_absolute = false);
		void CreateFilez(const BZKSTRING &fullname, bool is_absolute = false);
		void DefineRootPath(const BZKSTRING &path);
		BZKSTRING GetPath(const BZKSTRING &path);
		


	private:
		BZKSTRING path;

	};


	
}
