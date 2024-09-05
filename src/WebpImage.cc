#include "WebpImage.h"

#include <fstream>
#include <iostream>

WebpImage::~WebpImage()
{
    if (decoder != nullptr)
    {
        WebPAnimDecoderDelete(decoder);
    }
    else
    {
        WebPFree((void*)data.bytes);
    }
}

void
WebpImage::init(const char* filename)
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
    else std::cerr << "WebpImage: Error opening " << filename << std::endl;
}

bool
WebpImage::isWebp()
{
    return WebPGetInfo(data.bytes, data.size, NULL, NULL);
}

void
WebpImage::prepare()
{
    WebPAnimDecoderOptions options;
    WebPAnimDecoderOptionsInit(&options);
    options.color_mode = MODE_RGBA;
    decoder = WebPAnimDecoderNew(&data, &options);

    WebPAnimInfo webpInfo;
    WebPAnimDecoderGetInfo(decoder, &webpInfo);

    animate = (webpInfo.frame_count > 1);
    texture.create(webpInfo.canvas_width, webpInfo.canvas_height);
    sprite.setTexture(texture, true);

    prepareInfo("webp");
    valid = true;
}

void
WebpImage::update(const sf::Time& time)
{
    if (animate && lastFrameUpdate + sf::milliseconds(lastTimestamp) < time)
    {
        if (!WebPAnimDecoderHasMoreFrames(decoder))
        {
            WebPAnimDecoderReset(decoder);
        }

        uint8_t* pixels;
        WebPAnimDecoderGetNext(decoder, &pixels, &lastTimestamp);
        texture.update(pixels);
        lastFrameUpdate = time;
    }
}
