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
	BZKSTRING file = is_absolute ? fullname : GetPath(fullname);

	std::ofstream out(file.c_str());
	out.close();

	DWORD fileAttrs = GetFileAttributes(file.c_str());
	if(INVALID_FILE_ATTRIBUTES == fileAttrs && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		LoggerService::GetInstance().Log(L"File " + fullname + L" not created.");
	}
	else
	{
		LoggerService::GetInstance().Log(L"File " + fullname + L" created.");
	}
}

BZKSTRING BZK::FileSystem::GetPath(const BZKSTRING &newPath)
{
	const BZKCHAR slash = L'\\';
	
	// last index has slash
	if (path[path.length()] == slash && newPath[0] != slash)
		return path + newPath;
	if(path[path.length()] == slash && newPath[0] == slash)
		return path + newPath.substr(1);


	return path + slash + newPath;
}

void BZK::FileSystem::CreateDir(const BZKSTRING &name, bool is_absolute)
{
	if (GetFileAttributes(name.c_str()) & FILE_ATTRIBUTE_DIRECTORY)
	{
		BZKSTRING path = is_absolute ? name :GetPath(name);
		
		CreateDirectory(path.c_str(), NULL);

		LoggerService::GetInstance().Log(L"Directory " + name + L" created.");
	}
	else
		LoggerService::GetInstance().Log(L"Could not create directory");
}

void BZK::FileSystem::DefineRootPath(const BZKSTRING &path) { this->path = path; }