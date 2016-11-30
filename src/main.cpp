#include <iostream>
#include <map>
#include <vector>

#include "filesystem.h"

#include <unistd.h>

struct program_opts
{
	std::string dirpath;
};

struct program_opts parseOpts(int argc, char* argv[])
{
	program_opts opts{""};

	if (argc < 2)
		throw std::runtime_error("Please define WAV directory");

	opts.dirpath = argv[1];
	return opts;
}

int main(int argc, char* argv[])
{
	try
	{
		// get number of cpu
		auto numCPUs = sysconf(_SC_NPROCESSORS_ONLN);
		program_opts opts = parseOpts(argc, argv);
		// get directory's content
		auto files = filesystem::getFilesByPath(opts.dirpath);
		// filter only wav files
		auto wavFiles = filesystem::filterFileNames(files, "*.wav");

		// read files and encode it
		for (const auto& filepath : wavFiles)
		{
			std::cout << filepath << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}