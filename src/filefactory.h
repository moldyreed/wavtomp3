#ifndef FILEFACTORY_H
#define FILEFACTORY_H

#include "file.h"
#include <memory>
#include "wav.h"

enum format
{
	wav,
	mp3
};

std::unique_ptr<ifile> createFile(const std::string& fileName, format f)
{
	switch (f)
	{
		case wav:
			return std::make_unique<wavfile>(fileName);
			break;

		default:
			break;
	}
}

#endif // FILEFACTORY_H