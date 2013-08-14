#include "BZK_FileSystem.h"
#include <sstream>
#include <Windows.h>
#include <fstream>  


BZK::FileSystem::FileSystem()
{
}

BZK::FileSystem::~FileSystem()
{
}


void BZK::FileSystem::CreateFilez(const BZKSTRING &fullname, bool is_absolute)
{
	DWORD fileAttrs = GetFileAttributes(fullname.c_str());

	// check if file exists
	if(INVALID_FILE_ATTRIBUTES == fileAttrs && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		// file not found log

		return;
	}
	BZKSTRING file = is_absolute ? fullname : this->path + fullname;

	std::ofstream out(file.c_str());
	out.close();
}

void BZK::FileSystem::CreateDir(const BZKSTRING &name, bool is_absolute)
{
	if (GetFileAttributes(name.c_str()) & FILE_ATTRIBUTE_DIRECTORY)
	{
		BZKSTRING path = is_absolute ? name : this->path + name;
		CreateDirectory(path.c_str(), NULL);
	}
}

void BZK::FileSystem::DefineRootPath(const BZKSTRING &path) { this->path = path; }