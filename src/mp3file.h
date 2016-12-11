#ifndef MP3FILE_H
#define MP3FILE_H

#include <fstream>
#include "file.h"

class mp3file : public ifile
{
public:
	mp3file(mp3file&&) = default;
	mp3file(const mp3file&) = delete;
	mp3file(const std::string& filePath);
    virtual ~mp3file();

	std::size_t read(char* buffer, std::size_t bufferSize) {return 0;}
	void write(char* buffer, std::size_t bufferSize);

private:
	std::ofstream _outputFile;
};

#endif // MP3FILE_H
