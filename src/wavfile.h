#ifndef WAV_H
#define WAV_H

#include <fstream>
#include <vector>
#include "file.h"

class wavfile : public ifile
{
public:
    wavfile(wavfile&&) = default;
    wavfile(const wavfile&) = delete;
    wavfile(const std::string& filePath);
    virtual ~wavfile();

    std::size_t read(std::vector<std::int32_t>& inteleavedBuffer);
    std::size_t read(char* buffer, std::size_t bufferSize);
    void write(char* buffer, std::size_t bufferSize) {}

    struct WavHeader {
        unsigned char riff[4];						// RIFF string
        unsigned int overall_size	;				// overall size of file in bytes
        unsigned char wave[4];						// WAVE string
        unsigned char fmt_chunk_marker[4];			// fmt string with trailing null char
        unsigned int length_of_fmt;					// length of the format data
        unsigned int format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
        unsigned int channels;						// no.of channels
        unsigned int sample_rate;					// sampling rate (blocks per second)
        unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8
        unsigned int block_align;					// NumChannels * BitsPerSample/8
        unsigned int bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
        unsigned char data_chunk_header [4];		// DATA string or FLLR string
        unsigned int data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
        long bytes_in_each_channel;
        long size_of_each_sample;
        long num_samples;
    };

    WavHeader headers()
    {
        return _wavHeader;
    }

private:
    WavHeader _wavHeader;
    std::ifstream _inputFile;
    std::string _path;

    void readHeaders();
};

#endif // WAV_H
