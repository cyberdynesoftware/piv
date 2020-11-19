#pragma once

#include <SFML/Graphics.hpp>

class ImageView
{
    public:
        ImageView()
        {
            font.loadFromFile("font.ttf");
        }

        virtual void handle(sf::Event&) = 0;
        virtual void draw(void) = 0;

    protected:
        sf::Font font;
};
