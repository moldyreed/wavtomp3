#include <iostream>
#include <map>
#include <vector>

#include "filesystem.h"

struct program_opts
{
    std::string dirpath;
};

struct program_opts parseOpts(int argc, char *argv[])
{
    program_opts opts{""};
    if(argc != 2)
        throw std::runtime_error("Please define WAV directory");

    opts.dirpath = argv[1];
    return opts;
}

int main(int argc, char *argv[])
{
    try{
        program_opts opts = parseOpts(argc, argv);

        // get directory's content
        auto files = filesystem::getFilesByPath(opts.dirpath);

        // read files and encode it
        for(const auto& filepath : files)
        {
            std::cout << filepath << std::endl;
        }
    }
    catch(std::exception&e )
    {
        std::cerr << e.what() << std::endl;
    }

	return 0;
}
