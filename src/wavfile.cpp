#include "wavfile.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

wavfile::wavfile(const std::string& filePath) :
	_inputFile(filePath, std::ifstream::binary),
	_path(filePath),
	_wavHeader{0}
{
	if (!_inputFile)
		throw std::runtime_error("Can't open wav file: " + filePath);

	readHeaders();
}

void wavfile::readHeaders()
{
	unsigned char buffer4[4];
	unsigned char buffer2[2];
	int read = 0;
	// skip to WAV _wavHeader
	_inputFile.seekg(8);
	// read _wavHeader parts
	read = _inputFile.readsome((char*)&_wavHeader.wave, sizeof(_wavHeader.wave));

	if (strncasecmp((char*)_wavHeader.wave, "WAVE", 4) != 0)
		throw std::runtime_error("Wrong wave file format");

	_inputFile.seekg(0, std::ios_base::beg);
	_inputFile.seekg(22);
	read = _inputFile.readsome((char*)buffer2, sizeof(buffer2));
	_wavHeader.channels = buffer2[0] | (buffer2[1] << 8);
	_inputFile.seekg(0, std::ios_base::beg);
	_inputFile.seekg(24);
	read = _inputFile.readsome((char*)buffer4, sizeof(buffer4));
	_wavHeader.sample_rate = buffer4[0] |
							 (buffer4[1] << 8) |
							 (buffer4[2] << 16) |
							 (buffer4[3] << 24);
	_inputFile.seekg(0, std::ios_base::beg);
	_inputFile.seekg(34);
	read = _inputFile.readsome((char*)buffer2, sizeof(buffer2));
	_wavHeader.bits_per_sample = buffer2[0] |
								 (buffer2[1] << 8);
	read = _inputFile.readsome((char*)buffer4, sizeof(buffer4));
	_wavHeader.data_size = buffer4[0] |
						   (buffer4[1] << 8) |
						   (buffer4[2] << 16) |
						   (buffer4[3] << 24);
	_inputFile.seekg(0, std::ios_base::beg);
	_inputFile.seekg(41);
	read = _inputFile.readsome((char*)buffer4, sizeof(buffer4));
	_wavHeader.data_size = buffer4[0] |
						   (buffer4[1] << 8) |
						   (buffer4[2] << 16) |
						   (buffer4[3] << 24);
	_inputFile.seekg(0, std::ios_base::beg);
	_inputFile.seekg(44);
}

std::size_t wavfile::read(char* buffer, std::size_t bufferSize)
{
	return _inputFile.readsome(buffer, bufferSize);
}