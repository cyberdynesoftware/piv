#include "AnimatedGIF.h"
#include "ImageData.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include <stb/stb_image.h>

#include <iostream>

AnimatedGIF::AnimatedGIF()
{ }

bool
AnimatedGIF::test(const char* filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    stbi__context s;
    stbi__start_file(&s, f);
    bool result = stbi__gif_test(&s);
    fclose(f);
    return result;
}

void
AnimatedGIF::load(const char* filename)
{
    FILE *f = stbi__fopen(filename, "rb");
    stbi__context s;
    stbi__start_file(&s, f);

    int *delays;
    int comp = 0;

    void *pixels = stbi__load_gif_main(&s, &delays, &size.x, &size.y, &frameCount, &comp, STBI_rgb_alpha);

    sf::Image image;
    int step = size.x * size.y * 4;
    std::cout << "gif(kb): " << step * frameCount / 1000 << std::endl;

    for (int i = 0; i < frameCount; i++)
    {
        image.create(size.x, size.y, (const sf::Uint8*) pixels + step * i);

        int delay = delays[i];
        if (delay == 0 || delay == 10) delay = 100;

        frames.push_back(std::tuple<sf::Time, sf::Image>(sf::milliseconds(delay), image));
    }

    frameIter = frames.begin();
    
    stbi_image_free(pixels);
    fclose(f);
}

const sf::Time&
AnimatedGIF::delay()
{
    return std::get<0>(*frameIter);
}

void
AnimatedGIF::update(sf::Texture& texture)
{
    texture.loadFromImage(std::get<1>(*frameIter));
    frameIter++;
    if (frameIter == frames.end()) frameIter = frames.begin();
}
