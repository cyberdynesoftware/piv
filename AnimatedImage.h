#pragma once

#include <SFML/Graphics.hpp>

class AnimatedImage
{
    public:
        virtual ~AnimatedImage(void) { }

        virtual void update(sf::Texture&) = 0;
        
        bool animate = false;
        sf::Time delay;
};
