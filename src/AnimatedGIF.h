#pragma once

#include "Image.h"
#include "AnimatedImage.h"
#include <SFML/Graphics.hpp>

class AnimatedGIF : public Image, AnimatedImage
{
    public:
        AnimatedGIF();
        ~AnimatedGIF();

        virtual void init(const std::string& p);
        bool isGIF(void);
        virtual void prepare(void);
        virtual void load(const sf::Time& time);
        virtual void update(const sf::Time& time);

    private:
        struct stbi_pimpl* pimpl;
};
