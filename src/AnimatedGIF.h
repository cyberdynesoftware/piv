#pragma once

#include "Image.h"
#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedGIF : public Image
{
    public:
        AnimatedGIF();
        ~AnimatedGIF();

        virtual void init(const char*);
        bool isGIF(void);
        virtual void prepare(void);
        virtual void update(const sf::Time& time);

    private:
        struct stbi_pimpl* pimpl;

        struct Frame 
        {
            sf::Time delay;
            sf::Texture texture;
        };

        std::vector<Frame> frames;
        std::vector<Frame>::iterator frameIter;
        bool animate = false;
        sf::Time lastFrameUpdate = sf::milliseconds(0);
};
