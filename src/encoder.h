#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include <fstream>
#include <memory>

#include "lame.h"

#include "file.h"

class encoder
{
public:
    encoder(std::unique_ptr<ifile>&& input, std::unique_ptr<ifile>&& output);
    encoder(encoder&&) = default;
    ~encoder();

    void encode();

private:
    std::unique_ptr<ifile> _input;
    std::unique_ptr<ifile> _output;

    lame_global_flags* lgf;
};

#endif // ENCODER_H
