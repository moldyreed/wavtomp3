#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <vector>

class filesystem
{
public:
        static std::vector<std::string> getFilesByPath(const std::string& path, const std::string &filePattern);
	static bool isDirectory(const std::string& path);
};

#endif // FILESYSTEM_H
