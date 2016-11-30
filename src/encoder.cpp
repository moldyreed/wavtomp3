#include "encoder.h"

#include <stdexcept>

encoder::encoder(const std::string& filePath)
{
	lgf = lame_init();

	if (!lgf)
		throw std::runtime_error("Can't init lame codec");
}

encoder::~encoder()
{
	lame_close(lgf);
}