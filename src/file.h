#ifndef FILE_H
#define FILE_H

#include <iostream>

class ifile
{
public:
	virtual std::uint32_t read() = 0;
	virtual std::uint32_t write() = 0;

};

#endif // FILE_H