#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#define DIR_SEPARATOR "/"

class FileManager {
public:
	static bool SetContents(std::string Filename, std::string Content, bool Relative = true);

	static std::string GetContents(std::string Filename, bool Relative = true);

	static std::string GetCWD();

	static std::string GetFilenameWithoutExt(std::string Filename);

	static std::string GetFilenameExt(std::string Filename);
};

#endif
