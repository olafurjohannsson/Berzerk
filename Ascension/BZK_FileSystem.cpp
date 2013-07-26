#include "BZK_FileSystem.h"
#include <sstream>
#include <Windows.h>



BZK::FileSystem::FileSystem()
{
}

BZK::FileSystem::~FileSystem()
{
}



void BZK::FileSystem::CreateFilez(const BZKSTRING &fullname)
{
	// fullname should be: file.txt

	// check if file exists

	// if exists return 
	
	// if not exists, create file
}
void BZK::FileSystem::DefineRootPath(const BZKSTRING &path)
{
	this->path = path;



}

void BZK::FileSystem::CreateDir(const BZKSTRING &name)
{
	// Check if directory exists

	// If exists, return
	
	// If not exists, create folder 

	// auka:(má kalla í logservice með ("Folder Created Datetime.now")

}