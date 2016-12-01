#include "encoder.h"

#include <stdexcept>
#include "wavfile.h"

encoder::encoder(std::unique_ptr<ifile>&& input, std::unique_ptr<ifile>&& output) :
	_input(std::move(input)),
	_output(std::move(output))
{
	lgf = lame_init();
	auto params  = lame_init_params(lgf);

	if (!lgf && !params)
		throw std::runtime_error("Can't init lame codec");
}

encoder::~encoder()
{
	lame_close(lgf);
}

void encoder::encode()
{
	auto wavHeaders = dynamic_cast<wavfile&>(*_input).headers();
	auto chunkSize = wavHeaders.data_size;
	std::unique_ptr<char[]> readBuffer = std::make_unique<char[]>(chunkSize);
	auto numSamples = (8 * wavHeaders.data_size) / (wavHeaders.channels * wavHeaders.bits_per_sample);
	auto mp3bufsize = numSamples * 5 / 4 + 7200;
	std::unique_ptr<char[]> writeBuffer = std::make_unique<char[]>(mp3bufsize);
	size_t read = 0;

	while ((read = _input->read(readBuffer.get(), chunkSize)) > 0)
	{
//		int mp3size = lame_encode_buffer_interleaved(lgf,
//					  (short int*)readBuffer.get(),
//					  numSamples,
//					  (unsigned char*)writeBuffer.get(),
//					  (int)mp3bufsize);
//
//		short* leftPcm = new short[chunkSize / wavHeaders.channels / sizeof(short)];
//		short* rightPcm = new short[chunkSize / wavHeaders.channels / sizeof(short)];
		int mp3size = lame_encode_buffer(lgf,
										 (short*)readBuffer.get(),
										 (short*)readBuffer.get(),
										 numSamples,
										 (unsigned char*)writeBuffer.get(),
										 (int)mp3bufsize);
		int flushSize = lame_encode_flush(lgf, (unsigned char*)writeBuffer.get(), mp3size);
		_output->write(writeBuffer.get(), flushSize);
	}
}