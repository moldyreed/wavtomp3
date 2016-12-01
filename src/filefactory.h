#ifndef FILEFACTORY_H
#define FILEFACTORY_H

#include "file.h"
#include <memory>
#include "wavfile.h"
#include "mp3file.h"

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

		case mp3:
			return std::make_unique<mp3file>(fileName);
			break;

		default:
			break;
	}
}

#endif // FILEFACTORY_H