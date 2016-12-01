#ifndef FILE_H
#define FILE_H

#include <iostream>

class ifile
{
public:
	virtual std::size_t read(char* buffer, std::size_t bufferSize) = 0;
	virtual void write(char* buffer, std::size_t bufferSize) = 0;
};

#endif // FILE_H