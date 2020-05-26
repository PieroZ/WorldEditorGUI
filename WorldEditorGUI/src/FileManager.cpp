//=============================================================================
#include "FileManager.h"
//#include "Log.h"
#include "Stringify.h"

//#include <sys/param.h>
//#include "stdafx.h"
#include <stdio.h>

#include <direct.h>
#define GetCurrentDir _getcwd

//=============================================================================
bool FileManager::SetContents(std::string Filename, std::string Content, bool Relative)
{
	if (Filename == "") return false;

	if (Relative) Filename = GetCWD() + DIR_SEPARATOR + Filename;

	std::ofstream FileHandle;

	FileHandle.open(Filename.c_str());
	if (!FileHandle.is_open()) return false;

	FileHandle << Content;
	FileHandle.close();

	return true;
}

//-----------------------------------------------------------------------------
std::string FileManager::GetContents(std::string Filename, bool Relative)
{
	if (Filename == "") return "";

	if (Relative) Filename = GetCWD() + DIR_SEPARATOR + Filename;

	std::string Content;
	std::ifstream FileHandle;

	FileHandle.open(Filename.c_str());

	if (FileHandle.is_open()) {
		while (FileHandle.good()) {
			std::string Buffer;
			getline(FileHandle, Buffer);
			if (Buffer == "") continue;

			Content += Buffer + "\n";
		}

		FileHandle.close();
	}

	return Content;
}

//-----------------------------------------------------------------------------
std::string FileManager::GetCWD()
{
	std::string CWD;

#ifdef __APPLE__
	NSString* ResourcePath = [[NSBundle mainBundle]resourcePath];
	CWD = [ResourcePath cStringUsingEncoding : 1];
#else
	char Buffer[FILENAME_MAX];
	CWD = (_getcwd(Buffer, FILENAME_MAX) ? std::string(Buffer) : std::string(""));
#endif

	return CWD;
}

//-----------------------------------------------------------------------------
std::string FileManager::GetFilenameWithoutExt(std::string Filename)
{
	std::vector<std::string> Parts = Stringify::Explode(Filename, DIR_SEPARATOR);
	std::string NewFilename = Parts[Parts.size() - 1];

	// To Do: Filename could potentially have one or more dots
	Parts = Stringify::Explode(NewFilename, ".");
	NewFilename = Parts[0];

	return NewFilename;
}

//-----------------------------------------------------------------------------
std::string FileManager::GetFilenameExt(std::string Filename)
{
	std::vector<std::string> Parts = Stringify::Explode(Filename, ".");

	return (Parts.size() <= 1 ? "" : Parts[Parts.size() - 1]);
}

//=============================================================================
