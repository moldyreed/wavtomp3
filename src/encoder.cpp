#include "encoder.h"

#include <stdexcept>

encoder::encoder(std::unique_ptr<ifile>&& input, std::unique_ptr<ifile>&& output) :
	_input(std::move(input)),
	_output(std::move(output))
{
	lgf = lame_init();

	if (!lgf)
		throw std::runtime_error("Can't init lame codec");
}

encoder::~encoder()
{
	lame_close(lgf);
}

void encoder::encode()
{
}