#include "encoder.h"

#include <stdexcept>
#include "wavfile.h"

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

//    FILE *pcm = fopen(_input->path().c_str(), "rb");
//    FILE *mp3 = fopen(_output->path().c_str(), "wb");

//    int read, write;

//    auto wavHeaders = dynamic_cast<wavfile&>(*_input).headers();

//    lame_set_in_samplerate(lgf, wavHeaders.sample_rate);
//    lame_set_num_channels(lgf, wavHeaders.channels);
//    lame_set_num_samples(lgf, (8 * wavHeaders.data_size) / (wavHeaders.channels * wavHeaders.bits_per_sample));
//    lame_set_mode(lgf, STEREO);
//    lame_set_VBR(lgf, vbr_default);
//    auto params = lame_init_params(lgf);

//    if (params)
//        throw "pizda";

//    const int PCM_SIZE = 8192;
//    const int MP3_SIZE =  (1.25 * PCM_SIZE) + 7200;
//    int16_t pcm_buffer[PCM_SIZE*2];
//    u_char mp3_buffer[MP3_SIZE];

//    do {
//        read = fread(pcm_buffer, 2*sizeof(int16_t), PCM_SIZE, pcm);
//        if (read == 0)
//        {
//            write = lame_encode_flush(lgf, mp3_buffer, MP3_SIZE);
//        }
//        else
//        {
//            write = lame_encode_buffer_interleaved(lgf, (short*)pcm_buffer, read, mp3_buffer, MP3_SIZE);
//        }
//        fwrite(mp3_buffer, sizeof(char), write, mp3);
//    } while (read != 0);

//    lame_close(lgf);
//    fclose(mp3);
//    fclose(pcm);

    auto& input = dynamic_cast<wavfile&>(*_input);
    auto wavHeaders = input.headers();
    lame_set_num_channels(lgf, wavHeaders.channels);
//    lame_set_num_samples(lgf, (8 * wavHeaders.data_size) / (wavHeaders.channels * wavHeaders.bits_per_sample));
    lame_set_mode(lgf, STEREO);
    lame_set_VBR(lgf, vbr_default);
    auto params = lame_init_params(lgf);

    if (params)
        throw "pizda";

    auto numSamples = 8192;
    auto mp3bufsize = numSamples * 5 / 4 + 7200;

    std::vector<short> readBuffer;
    readBuffer.reserve(numSamples * 2);
    std::unique_ptr<unsigned char[]> wBuffer = std::make_unique<unsigned char[]>(mp3bufsize);

    size_t read = 0, write = 0;

    while ((read = input.read(readBuffer)) > 0)
    {
        write = lame_encode_buffer_interleaved(lgf, (short*)readBuffer.data(), read / 2, wBuffer.get(), (int)mp3bufsize);
        if(write <= 0)
        {
            std::cerr << "Vse ploho: " << write << std::endl;
            exit(1);
        }
        _output->write((char*)wBuffer.get(), write);
    }

    int flushSize = lame_encode_flush(lgf, wBuffer.get(), write);
    _output->write((char*)wBuffer.get(), flushSize);
}
