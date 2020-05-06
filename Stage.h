#pragma once

#include <SFML/Graphics.hpp>

class Stage
{
    public:
        virtual void handle(sf::Event&) = 0;
        virtual void draw(void) = 0;
};
