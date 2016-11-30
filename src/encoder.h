#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include <fstream>
#include <memory>

#include <lame/lame.h>

class encoder
{
public:
	encoder(const std::string& filePath);
	~encoder();
private:
	std::ifstream inputFile;
	std::ofstream outputFile;

	lame_global_flags* lgf;
};

#endif // ENCODER_H