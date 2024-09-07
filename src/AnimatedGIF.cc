#include "AnimatedGIF.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include "lib/stb_image.h"

#include <fstream>
#include <iostream>

struct stbi_pimpl
{
    FILE *file = NULL;
    stbi__context context;
};

AnimatedGIF::AnimatedGIF()
{
    pimpl = new stbi_pimpl;
}

void
AnimatedGIF::init(const std::string& p)
{
    path = p;
    pimpl->file = stbi__fopen(path.c_str(), "rb");
    stbi__start_file(&pimpl->context, pimpl->file);
}

AnimatedGIF::~AnimatedGIF()
{
    if (pimpl->file != NULL)
        fclose(pimpl->file);

    delete pimpl;
}

bool
AnimatedGIF::isGIF()
{
    return stbi__gif_test(&pimpl->context);
}

void
AnimatedGIF::prepare()
{
    int x, y, frameCount;
    int *delays;
    int comp = 0;

    void *pixels = stbi__load_gif_main(&pimpl->context, &delays, &x, &y, &frameCount, &comp, STBI_rgb_alpha);

    int step = x * y * 4;

    for (int i = 0; i < frameCount; i++)
    {
        Frame frame;
        frame.delay = sf::milliseconds(delays[i] <= 10 ? 100 : delays[i]);
        frame.texture.create(x, y);
        frame.texture.setSmooth(true);
        frame.texture.update((sf::Uint8*) pixels + step * i);

        frames.push_back(frame);
    }

    animate = (frameCount > 1);

    stbi_image_free(pixels);
    delete[] delays;
    fclose(pimpl->file);
    pimpl->file = NULL;

    frameIter = frames.begin();
    sprite.setTexture(frameIter->texture, false);
    prepareInfo("gif");
    valid = true;
}

void
AnimatedGIF::load(const sf::Time& time)
{
    sprite.setTexture(frameIter->texture, false);
    lastFrameUpdate = time;
}

void
AnimatedGIF::update(const sf::Time& time)
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
