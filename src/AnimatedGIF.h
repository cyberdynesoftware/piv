#pragma once

#include "Image.h"
#include "AnimatedImage.h"
#include <SFML/Graphics.hpp>

class AnimatedGIF : public AnimatedImage
{
    public:
        AnimatedGIF();
        ~AnimatedGIF();

        virtual void init(const std::string& p);
        bool isGIF(void);
        virtual void prepare(void);

    private:
        struct stbi_pimpl* pimpl;
};
