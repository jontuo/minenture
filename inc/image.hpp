
#ifndef __IMAGE_HPP_INCLUDED__
#define __IMAGE_HPP_INCLUDED__

#include "stb_image.h"



/*
    Currently supports 8 bits per channel and 1-4 channels.

    TODO: Support for loading from memory (stbi_load_from_memory).
    TODO: Support for 16 bits per channel (stbi_load_16).

    if(data == NULL) { throw(stbi_failure_reason()); }
*/

class Image
{
public:

    Image();
    Image(const char* filePath);
    ~Image();

    signed int getWidth();
    signed int getHeight();
    signed int getChannels();
    unsigned char* getData();

private:
    unsigned char* data;
    signed int width;
    signed int height;
    signed int channels;
};

#endif
