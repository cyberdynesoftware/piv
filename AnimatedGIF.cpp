#include "AnimatedGIF.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include "stb_image.h"

#include <fstream>
#include <iostream>

struct stbi_pimpl
{
    FILE *file;
    stbi__context context;
};

AnimatedGIF::AnimatedGIF(const char* filename)
{
    pimpl = new stbi_pimpl;
    pimpl->file = stbi__fopen(filename, "rb");
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
AnimatedGIF::load()
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

    if (frameCount > 1)
        animate = true;

    frameIter = frames.begin();

    stbi_image_free(pixels);
    delete[] delays;
    fclose(pimpl->file);
    pimpl->file = NULL;
}

void
AnimatedGIF::update(sf::Sprite& sprite)
{
    sprite.setTexture(frameIter->texture, true);
    delay = frameIter->delay;

    frameIter++;
    
    if (frameIter == frames.end())
        frameIter = frames.begin();
}
