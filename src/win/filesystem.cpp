#include "filesystem.h"

#include <unistd.h>
#include <limits.h>
#include <windows.h>

std::vector<std::string> filesystem::getFilesByPath(const std::string& path, const std::string& filePattern)
{
    std::vector<std::string> files;

    WIN32_FIND_DATA fdFile{0};
    HANDLE hFind = NULL;

    if(isDirectory(path)) {
        std::string newPath(path + "\\" + filePattern);
        hFind = FindFirstFile(newPath.c_str(), &fdFile);

        if (INVALID_HANDLE_VALUE == hFind) {
            throw std::runtime_error("Something went wrong while path reading: " +  path);
        }

        do {
            if (!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                files.emplace_back(path + "\\" + fdFile.cFileName);
            }
        } while (FindNextFile(hFind, &fdFile) != 0);


        auto dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
            std::cout << dwError << std::endl;
        }

        FindClose(hFind);


    } else {
        throw std::runtime_error("Path not found: " +  path + " or it's not a directory.");
    }
    return files;
}

bool filesystem::isDirectory(const std::string& path)
{
    DWORD ftyp = GetFileAttributesA(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }

    return false;
}
