#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <vector>

class filesystem
{
public:
	static std::vector<std::string> getFilesByPath(const std::string& path);
	static bool isDirectory(const std::string& path);
	static std::vector<std::string> filterFileNames(const std::vector<std::string>& fileNames, const std::string &pattern = "*");
};

#endif // FILESYSTEM_H