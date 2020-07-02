#include "AnimatedGIF.h"
#include "ImageData.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include <stb/stb_image.h>

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
    int z = 0, comp = 0;

    void *pixels = stbi__load_gif_main(&s, &delays, &size.x, &size.y, &z, &comp, STBI_rgb_alpha);

    sf::Image image;
    int step = size.x * size.y * 4;

    for (int i = 0; i < z; i++)
    {
        image.create(size.x, size.y, (const sf::Uint8*) pixels + step * i);

        sf::Texture texture;
        texture.loadFromImage(image);
        texture.setSmooth(true);

        int delay = delays[i];
        if (delay == 0 || delay == 10) delay = 100;

        frames.push_back(std::tuple<sf::Time, sf::Texture>(sf::milliseconds(delay), texture));
    }

    frameIter = frames.begin();
    
    stbi_image_free(pixels);
    fclose(f);

    totalDelay = sf::Time::Zero;
    startTime = clock.getElapsedTime();
}

const sf::Vector2i&
AnimatedGIF::getSize()
{
    return size;
}

void
AnimatedGIF::update(sf::Sprite& sprite, bool square)
{
    sf::Time& delay = std::get<0>(*frameIter);

    while (startTime + totalDelay + delay < clock.getElapsedTime())
    {
        frameIter++;
        if (frameIter == frames.end()) frameIter = frames.begin();
        totalDelay += delay;
        delay = std::get<0>(*frameIter);
    }

    // TODO: only when dirty
    sf::Texture& texture = std::get<1>(*frameIter);
    sprite.setTexture(texture, true);
    if (square) ImageData::square(sprite);
}
