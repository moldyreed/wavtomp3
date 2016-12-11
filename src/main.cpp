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

struct program_opts {
    std::string dirpath;
};

struct program_opts parseOpts(int argc, char* argv[])
{
    program_opts opts{""};

    if (argc < 2) {
        throw std::runtime_error("Please define WAV directory");
    }

    opts.dirpath = argv[1];
    return opts;
}

std::string makeMP3FilePath(const std::string& filePath)
{
    return filePath.substr(0, filePath.length() - 3) + "mp3";
}

void* runTask(void* arg)
{
    try {
        AsyncQueue<std::string>* queue = reinterpret_cast<AsyncQueue<std::string>*>(arg);
        if(queue->hasMoreItems()) {
            const auto filepath = queue->pop();
            const auto mp3path = makeMP3FilePath(filepath);
            encoder enc(createFile(filepath, format::wav), createFile(mp3path, format::mp3));
            enc.encode();
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return (void *)0;
}

int main(int argc, char* argv[])
{
    try {
        // get number of cpu
        auto numCPUs = std::thread::hardware_concurrency();
        program_opts opts = parseOpts(argc, argv);
        // get directory's content and filter only wav files
        auto wavFiles = filesystem::getFilesByPath(opts.dirpath, "*.wav");

        if(wavFiles.size() == 0) {
            throw std::runtime_error("No wav files in: " + opts.dirpath);
        }

        AsyncQueue<std::string> filePathEncodeQueue;

        // add filepaths into async queue
        for (const auto& filepath : wavFiles) {
            std::cout << "File: " << filepath << " will be encoded" << std::endl;
            filePathEncodeQueue.push(filepath);
        }

        // create thread pool
        pthread_t threads[numCPUs];
        for (auto i = 0; i < numCPUs; i++) {
            auto ret = pthread_create(&threads[i], NULL, runTask, (void*)&filePathEncodeQueue);

            if(ret != 0) {
                throw std::runtime_error("Cannot create new thread.");
            }
        }

        // join all thread into main thread
        for (auto i = 0; i < numCPUs; i++) {
            auto ret = pthread_join(threads[i], NULL);
            if (ret != 0) {
                throw std::runtime_error("A POSIX thread error occured.");
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
