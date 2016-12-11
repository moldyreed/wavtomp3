#ifndef FILE_H
#define FILE_H

#include <iostream>

class ifile
{
public:
        ifile(const std::string& path) : _path(path){}
        virtual ~ifile(){}

        std::string path(){ return _path; }

	virtual std::size_t read(char* buffer, std::size_t bufferSize) = 0;
        virtual void write(char* buffer, std::size_t bufferSize) = 0;
protected:
    const std::string _path;
};

#endif // FILE_H
