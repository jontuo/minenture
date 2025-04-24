
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG // User-friendly error messages from "stb_image.h".
#include "image.hpp"



Image::Image(const char* filePath):
    data(stbi_load(filePath, &width, &height, &channels, 0))
{

}

Image::~Image()
{
    stbi_image_free(data);
}

signed int Image::getWidth()
{
    return(width);
}

signed int Image::getHeight()
{
    return(height);
}

signed int Image::getChannels()
{
    return(channels);
}

unsigned char* Image::getData()
{
    return(data);
}
