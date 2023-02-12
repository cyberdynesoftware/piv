#pragma once

#include <SFML/Graphics.hpp>

class AnimatedImage
{
    public:
        virtual ~AnimatedImage(void) { }

        virtual void update(sf::Time, sf::Sprite&) = 0;
        
        bool animate = false;
        sf::Time delay = sf::milliseconds(0);
};
