#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <thread>
#include <pthread.h>

#include "filesystem.h"
#include "encoder.h"
#include "filefactory.h"
#include "asyncqueue.hpp"

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

std::string makeMP3FilePath(const std::string& filePath)
{
	return filePath.substr(0, filePath.length() - 3) + "mp3";
}

void* runTask(void* arg)
{
	AsyncQueue<std::string>* queue = (AsyncQueue<std::string>*)arg;
	const auto filepath = queue->pop();
	const auto mp3path = makeMP3FilePath(filepath);
	auto input = createFile(filepath, format::wav);
	auto output = createFile(mp3path, format::mp3);
//	encoder enc(*input, *output);
//	enc.encode();
	std::cout << mp3path << std::endl;
}

int main(int argc, char* argv[])
{
	try
	{
		// get number of cpu
		auto numCPUs = std::thread::hardware_concurrency();
		program_opts opts = parseOpts(argc, argv);
		// get directory's content
		auto files = filesystem::getFilesByPath(opts.dirpath);
		// filter only wav files
		auto wavFiles = filesystem::filterFileNames(files, "*.wav");
		AsyncQueue<std::string> queue;

		// read files and encode it
		for (const auto& filepath : wavFiles)
		{
			queue.push(filepath);
		}

		pthread_t threads[numCPUs];

		for (auto i = 0; i < numCPUs; i++)
		{
			pthread_create(&threads[i], NULL, runTask, (void*)&queue);
		}

		for (auto i = 0; i < numCPUs; i++)
		{
			int ret = pthread_join(threads[i], NULL);

			if (ret != 0)
			{
				std::cerr << "A POSIX thread error occured." << std::endl;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}