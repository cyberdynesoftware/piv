#include "WebpImage.h"

#include <fstream>
#include <iostream>

WebpImage::WebpImage(const char* filename)
{
    loadFile(filename);
}

WebpImage::~WebpImage()
{
    if (decoder != nullptr)
        WebPAnimDecoderDelete(decoder);
    else
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
WebpImage::prepare(sf::Sprite& sprite)
{
    WebPAnimDecoderOptions options;
    WebPAnimDecoderOptionsInit(&options);
    options.color_mode = MODE_RGBA;
    decoder = WebPAnimDecoderNew(&data, &options);

    WebPAnimInfo info;
    WebPAnimDecoderGetInfo(decoder, &info);

    animate = (info.frame_count > 1);
    texture.create(info.canvas_width, info.canvas_height);
    sprite.setTexture(texture, true);
}

void
WebpImage::update(sf::Time time, sf::Sprite& sprite)
{
    if (!WebPAnimDecoderHasMoreFrames(decoder))
    {
        WebPAnimDecoderReset(decoder);
        previousTimestamp = 0;
    }

    uint8_t* pixels;
    int timestamp;
    WebPAnimDecoderGetNext(decoder, &pixels, &timestamp);

    texture.update(pixels);

    while (delay < time)
        delay += sf::milliseconds(timestamp - previousTimestamp);
    previousTimestamp = timestamp;
}
