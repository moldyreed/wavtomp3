#include "encoder.h"

#include <stdexcept>
#include "wavfile.h"

encoder::encoder(std::unique_ptr<ifile>&& input, std::unique_ptr<ifile>&& output) :
    _input(std::move(input)),
    _output(std::move(output))
{
    lgf = lame_init();

    if (!lgf) {
        throw std::runtime_error("Can't init lame codec");
    }
}

encoder::~encoder()
{
    lame_close(lgf);
}

void encoder::encode()
{
    auto& input = dynamic_cast<wavfile&>(*_input);
    auto wavHeaders = input.headers();
    lame_set_num_channels(lgf, wavHeaders.channels);
    lame_set_analysis(lgf, 1);
    auto params = lame_init_params(lgf);

    if (params) {
        throw std::runtime_error("Can't init lame with such parameters");
    }

    auto samples_to_read = lame_get_framesize(lgf);
    std::vector<std::int32_t> readBuffer;
    readBuffer.reserve(samples_to_read * 2);
    std::unique_ptr<unsigned char[]> wBuffer = std::make_unique<unsigned char[]>(LAME_MAXMP3BUFFER);

    auto read = 0, write = 0;
    // TODO pass more or less two channels

    std::vector<std::int32_t> lpcm;
    std::vector<std::int32_t> rpcm;
    lpcm.reserve(readBuffer.capacity()/2);
    rpcm.reserve(readBuffer.capacity()/2);

    while ((read = input.read(readBuffer)) > 0) {

        lpcm.clear();
        rpcm.clear();

        for(auto i = 0 ; i <= read; i += 2) {
            lpcm.push_back( readBuffer[i] );
            rpcm.push_back( readBuffer[i+1] );
        }
        write = lame_encode_buffer_int(lgf, lpcm.data(), rpcm.data(), read / wavHeaders.channels, wBuffer.get(), LAME_MAXMP3BUFFER);
        if(write < 0) {
            throw std::runtime_error("All go wrong");
        }
        _output->write(reinterpret_cast<char*>(wBuffer.get()), write);
    }

    auto flushSize = lame_encode_flush(lgf, wBuffer.get(), LAME_MAXMP3BUFFER);
    _output->write(reinterpret_cast<char*>(wBuffer.get()), flushSize);
}
