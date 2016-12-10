#include "wavfile.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

wavfile::wavfile(const std::string& filePath) :
    ifile(filePath),
    _inputFile(filePath, std::ifstream::binary),
    _wavHeader{0}
{
    if (!_inputFile && !_inputFile.is_open()) {
        throw std::runtime_error("Can't open wav file: " + filePath);
    }

    readHeaders();
}

std::size_t wavfile::read(std::vector<std::int32_t> &inteleavedBuffer)
{
    inteleavedBuffer.clear();
    auto i = 0;
    const int b = sizeof(int) * 8;
    for(; i < inteleavedBuffer.capacity(); i++) {
        std::int32_t datum = 0;

        unsigned char chars[_wavHeader.bits_per_sample / 8];
        _inputFile.read((char*)chars, sizeof(chars));
        if (_inputFile.eof() == true) {
            return i;
        }

        // convet to 32 bit int
        switch (_wavHeader.bits_per_sample) {
        case 32:
            datum = chars[0] << (b - 32) | chars[1] << (b - 24) | chars[2] << (b - 16) | chars[3] << (b - 8);
            break;
        case 24:
            datum = chars[0] << (b - 24) | chars[1] << (b - 16) | chars[2] << (b - 8);
            break;
        case 16:
            datum = chars[0] << (b - 16) | chars[1] << (b - 8);
            break;
        case 8:
            datum = chars[0] << (b - 8);
            break;
        }

        inteleavedBuffer.push_back(datum);

    }
    return i;
}

void wavfile::readHeaders()
{
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    _inputFile.read((char*)_wavHeader.riff, sizeof(_wavHeader.riff));
    _inputFile.read((char*)buffer4, sizeof(buffer4));

    // convert little endian to big endian 4 byte int
    _wavHeader.overall_size  = buffer4[0] |
                               (buffer4[1]<<8) |
                               (buffer4[2]<<16) |
                               (buffer4[3]<<24);
    _inputFile.read((char*)_wavHeader.wave, sizeof(_wavHeader.wave));

    _inputFile.read((char*)_wavHeader.fmt_chunk_marker, sizeof(_wavHeader.fmt_chunk_marker));

    _inputFile.read((char*)buffer4, sizeof(buffer4));
    _wavHeader.length_of_fmt = buffer4[0] |
                               (buffer4[1] << 8) |
                               (buffer4[2] << 16) |
                               (buffer4[3] << 24);
    _inputFile.read((char*)buffer2, sizeof(buffer2));

    _wavHeader.format_type = buffer2[0] | (buffer2[1] << 8);
    char format_name[10] = "";
    if (_wavHeader.format_type == 1) {
        strcpy(format_name,"PCM");
    } else if (_wavHeader.format_type == 6) {
        strcpy(format_name, "A-law");
    } else if (_wavHeader.format_type == 7) {
        strcpy(format_name, "Mu-law");
    }

    printf("(21-22) Format type: %u %s \n", _wavHeader.format_type, format_name);

    _inputFile.read((char*)buffer2, sizeof(buffer2));
    _wavHeader.channels = buffer2[0] | (buffer2[1] << 8);

    _inputFile.read((char*)buffer4, sizeof(buffer4));
    _wavHeader.sample_rate = buffer4[0] |
                             (buffer4[1] << 8) |
                             (buffer4[2] << 16) |
                             (buffer4[3] << 24);
    _inputFile.read((char*)buffer4, sizeof(buffer4));
    _wavHeader.byterate  = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24);
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", _wavHeader.byterate, _wavHeader.byterate*8);

    _inputFile.read((char*)buffer2, sizeof(buffer2));
    _wavHeader.block_align = buffer2[0] |
                             (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", _wavHeader.block_align);

    _inputFile.read((char*)buffer2, sizeof(buffer2));
    _wavHeader.bits_per_sample = buffer2[0] |
                                 (buffer2[1] << 8);
    printf("(35-36) Bits per sample: %u \n", _wavHeader.bits_per_sample);

    _inputFile.read((char*)_wavHeader.data_chunk_header, sizeof(_wavHeader.data_chunk_header));
    printf("(37-40) Data Marker: %s \n", _wavHeader.data_chunk_header);

    _inputFile.read((char*)buffer4, sizeof(buffer4));
    _wavHeader.data_size = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24 );
    printf("(41-44) Size of data chunk: %u \n", _wavHeader.data_size);
    _wavHeader.size_of_each_sample = (_wavHeader.channels * _wavHeader.bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", _wavHeader.size_of_each_sample);
    float duration_in_seconds = (float) _wavHeader.overall_size / _wavHeader.byterate;
    printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
    _wavHeader.bytes_in_each_channel = ( _wavHeader.size_of_each_sample / _wavHeader.channels);
    _wavHeader.num_samples = (8 * _wavHeader.data_size) / (_wavHeader.channels * _wavHeader.bits_per_sample);

    // the valid amplitude range for values based on the bits per sample
    long low_limit = 0l;
    long high_limit = 0l;

    switch (_wavHeader.bits_per_sample) {
    case 8:
        low_limit = INT8_MIN;
        high_limit = INT8_MAX;
        break;
    case 16:
        low_limit = INT16_MIN;
        high_limit = INT16_MAX;
        break;
    case 24:
    case 32:
        low_limit = INT32_MIN;
        high_limit = INT32_MAX;
        break;
    }
}

std::size_t wavfile::read(char* buffer, std::size_t bufferSize)
{
    char data_buffer[_wavHeader.size_of_each_sample];
    for(auto i = 0 ; i < bufferSize; i += sizeof(data_buffer)) {
        _inputFile.read(data_buffer, sizeof(data_buffer));
        if (_inputFile.eof() != true) {

            // dump the data read
            int data_in_channel = 0;

            for (auto xchannels = 0; xchannels < _wavHeader.channels; xchannels ++ ) {
                printf("Channel#%d : ", (xchannels+1));
                // convert data from little endian to big endian based on bytes in each channel sample
                if (_wavHeader.bytes_in_each_channel == 4) {
                    data_in_channel =	data_buffer[0] |
                                        (data_buffer[1]<<8) |
                                        (data_buffer[2]<<16) |
                                        (data_buffer[3]<<24);
                } else if (_wavHeader.bytes_in_each_channel == 3) {
                    data_in_channel =	data_buffer[0] |
                                        (data_buffer[1]<<8) |
                                        (data_buffer[2]<<16);
                } else if (_wavHeader.bytes_in_each_channel == 2) {
                    data_in_channel = data_buffer[0] |
                                      (data_buffer[1] << 8);
                } else if (_wavHeader.bytes_in_each_channel == 1) {
                    data_in_channel = data_buffer[0];
                }


            }
        }
    }

    auto& streamSize = _inputFile.read(buffer, bufferSize);
    return streamSize.gcount();
}
