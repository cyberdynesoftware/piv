#include "WebpImage.h"

#include <fstream>
#include <iostream>

WebpImage::WebpImage(const char* filename)
{
    loadFile(filename);
}

WebpImage::~WebpImage()
{
    WebPFree((void*)data.bytes);
}

void
WebpImage::loadFile(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        data.size = in.tellg();
        data.bytes = (uint8_t*)WebPMalloc(data.size);
        in.seekg(0, std::ios::beg);
        in.read((char*)data.bytes, data.size);
        in.close();
    }
    else std::cerr << "Error opening " << filename << std::endl;
}

bool
WebpImage::isWebp()
{
    return WebPGetInfo(data.bytes, data.size, NULL, NULL);
}

void
WebpImage::update(sf::Texture& texture)
{
}
