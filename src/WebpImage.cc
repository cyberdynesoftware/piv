#include "WebpImage.h"

#include <fstream>
#include <iostream>

WebpImage::~WebpImage()
{
    WebPFree((void*)data.bytes);
}

void
WebpImage::init(const std::string& p)
{
    path = p;
    std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        data.size = in.tellg();
        data.bytes = (uint8_t*)WebPMalloc(data.size);
        in.seekg(0, std::ios::beg);
        in.read((char*)data.bytes, data.size);
        in.close();
    }
    else std::cerr << "WebpImage: Error opening " << path << std::endl;
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
    WebPAnimDecoder* decoder = WebPAnimDecoderNew(&data, &options);

    WebPAnimInfo webpInfo;
    WebPAnimDecoderGetInfo(decoder, &webpInfo);
    animate = (webpInfo.frame_count > 1);

    uint8_t* pixels;
    int timestamp;

    while (WebPAnimDecoderHasMoreFrames(decoder))
    {
        WebPAnimDecoderGetNext(decoder, &pixels, &timestamp);

        Frame frame;
        frame.delay = sf::milliseconds(timestamp);
        frame.texture.create(webpInfo.canvas_width, webpInfo.canvas_height);
        frame.texture.setSmooth(true);
        frame.texture.update(pixels);
        
        frames.push_back(frame);
    }

    auto delay = frames[1].delay - frames[0].delay;
    std::for_each(frames.begin(), frames.end(), [&delay](auto& frame) { frame.delay = delay; });

    WebPAnimDecoderDelete(decoder);

    frameIter = frames.begin();
    sprite.setTexture(frameIter->texture, false);
    prepareInfo("webp");
    valid = true;
}

void
WebpImage::load(const sf::Time& time)
{
    sprite.setTexture(frameIter->texture, false);
    lastFrameUpdate = time;
}

void
WebpImage::update(const sf::Time& time)
{
    if (animate && lastFrameUpdate + frameIter->delay < time)
    {
        if (++frameIter == frames.end())
        {
            frameIter = frames.begin();
        }

        load(time);
    }
}
