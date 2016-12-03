#include "filesystem.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <fnmatch.h>

std::vector<std::string> filesystem::getFilesByPath(const std::string& path)
{
	std::vector<std::string> files;

	if (isDirectory(path))
	{
		char fileName[NAME_MAX] ;
		struct dirent* dp;
		DIR* dfd;

		if ((dfd = opendir(path.c_str())) == NULL)
		{
			throw std::logic_error("Can't open directory: " + path);
		}

		while ((dp = readdir(dfd)) != NULL)
		{
			struct stat st {0};
			sprintf(fileName , "%s/%s", path.c_str(), dp->d_name) ;

			if (stat(fileName, &st) == -1)
			{
				std::cerr << "Unable to stat file: " << fileName << "\n";
				continue ;
			}

			if (S_ISDIR(st.st_mode))
			{
				continue;
			}
			else
			{
				files.emplace_back(fileName);
			}
		}
	}
	else
		throw std::logic_error("Not a directory");

	return files;
}

bool filesystem::isDirectory(const std::string& path)
{
	struct stat pathStat {0};

	if (stat(path.c_str(), &pathStat) != 0)
	{
		throw std::logic_error("Directory not exists");
	}

	return S_ISDIR(pathStat.st_mode);
}

std::vector<std::string> filesystem::filterFileNames(const std::vector<std::string>& fileNames, const std::string& pattern)
{
	std::vector<std::string> filteredFileNames;

	for (const auto& fileName : fileNames)
	{
		// match path tand call method
		if (fnmatch(pattern.c_str(), fileName.c_str(), 0) == 0)
			filteredFileNames.emplace_back(fileName);
	}

	return filteredFileNames;
}
