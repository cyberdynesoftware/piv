#include "AnimatedGIF.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include <stb/stb_image.h>

struct AnimatedGIF::stbi_var
{
    stbi__context s;
};

AnimatedGIF::AnimatedGIF(const char* filename)
{
    pImpl = new stbi_var;

    FILE *f = stbi__fopen(filename, "rb");
    stbi__start_file(&pImpl->s, f);
}

AnimatedGIF::~AnimatedGIF()
{
    delete pImpl;
}

bool
AnimatedGIF::test()
{
    return stbi__gif_test(&pImpl->s);
}

void
AnimatedGIF::load()
{
    int *delays;
    int z = 0, comp = 0;

    void *pixels = stbi__load_gif_main(&pImpl->s, &delays, &size.x, &size.y, &z, &comp, STBI_rgb_alpha);

    sf::Image image;
    int step = size.x * size.y * 4;

    for (int i = 0; i < z; i++)
    {
        image.create(size.x, size.y, (const sf::Uint8*) pixels + step * i);

        sf::Texture texture;
        texture.loadFromImage(image);

        int delay = delays[i];
        if (delay == 0 || delay == 10) delay = 100;

        frames.push_back(std::tuple<sf::Time, sf::Texture>(sf::milliseconds(delay), texture));
    }

    frameIter = frames.begin();
    
    stbi_image_free(pixels);

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
}
