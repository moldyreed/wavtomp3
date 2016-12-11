#include "filesystem.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <fnmatch.h>

std::vector<std::string> filesystem::getFilesByPath(const std::string& path, const std::string &filePattern)
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
                if (fnmatch(filePattern.c_str(), dp->d_name, 0) == 0)
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
