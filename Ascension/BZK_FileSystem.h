#include "BZK_Typedef.h"
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
		FileSystem(const BZKSTRING &p) : path(p) {};


		~FileSystem();

		

		void CreateFilez(const BZKSTRING &fullname);
		void DefineRootPath(const BZKSTRING &path);
		void CreateDir(const BZKSTRING &name);
		


	private:
		BZKSTRING path;

	};


	
}
