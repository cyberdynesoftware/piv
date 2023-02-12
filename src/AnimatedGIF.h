#pragma once

#include "AnimatedImage.h"
#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedGIF : public AnimatedImage
{
    public:
        AnimatedGIF(const char*);
        ~AnimatedGIF();

        bool isGIF(void);
        void load(void);
        void update(sf::Time, sf::Sprite&);

    private:
        struct stbi_pimpl* pimpl;

        struct Frame 
        {
            sf::Time delay;
            sf::Texture texture;
        };

        std::vector<Frame> frames;
        std::vector<Frame>::iterator frameIter;
};
