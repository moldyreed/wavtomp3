#include "mp3file.h"

mp3file::mp3file(const std::string& filePath) :
    ifile(filePath),
	_outputFile(filePath, std::ofstream::binary)
{
	if (!_outputFile)
		throw std::runtime_error("Can't open mp3 file: " + filePath);
}

void mp3file::write(char* buffer, std::size_t bufferSize)
{
	_outputFile.write(buffer, bufferSize);
}
