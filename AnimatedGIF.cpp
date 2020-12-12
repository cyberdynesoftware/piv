#include "AnimatedGIF.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include "stb_image.h"

#include <fstream>
#include <iostream>

struct stbi_pimpl
{
    stbi__context context;
    stbi__gif gif;
    int comp = 0;
};

AnimatedGIF::AnimatedGIF(const char* filename)
{
    pimpl = new stbi_pimpl;
    memset(&pimpl->gif, 0, sizeof(pimpl->gif));
    loadFile(filename);
    stbi__start_mem(&pimpl->context, (stbi_uc*)fileBuffer, filesize);
    animate = true;
}

AnimatedGIF::~AnimatedGIF()
{
    STBI_FREE(pimpl->gif.out); 
    STBI_FREE(pimpl->gif.history); 
    STBI_FREE(pimpl->gif.background); 
    delete pimpl;
    delete[] fileBuffer;
}

void
AnimatedGIF::loadFile(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        filesize = in.tellg();
        fileBuffer = new char[filesize];
        in.seekg(0, std::ios::beg);
        in.read(fileBuffer, filesize);
        in.close();
    }
    else std::cerr << "Error opening " << filename << std::endl;
}

bool
AnimatedGIF::isGIF()
{
    return stbi__gif_test(&pimpl->context);
}

void
AnimatedGIF::update(sf::Texture& texture)
{
    stbi_uc* pixels = stbi__gif_load_next(&pimpl->context, &pimpl->gif, &pimpl->comp, STBI_rgb_alpha, 0);

    // returning the context signals all frames have been read
    if (pixels == (stbi_uc*)&pimpl->context)
    {
        STBI_FREE(pimpl->gif.out); 
        STBI_FREE(pimpl->gif.history); 
        STBI_FREE(pimpl->gif.background); 
        memset(&pimpl->gif, 0, sizeof(pimpl->gif));
        stbi__start_mem(&pimpl->context, (const stbi_uc*)fileBuffer, filesize);
        delay = sf::milliseconds(0);
    }
    else
    {
        unsigned int x = pimpl->gif.w;
        unsigned int y = pimpl->gif.h;

        if (texture.getSize().x != x || texture.getSize().y != y)
            texture.create(x, y);

        texture.update(pixels);
        int temp_delay = pimpl->gif.delay;
        if (temp_delay <= 10) temp_delay = 100;
        delay = sf::milliseconds(temp_delay);
    }
}
