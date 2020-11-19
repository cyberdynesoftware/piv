#pragma once

#include <SFML/Graphics.hpp>

class Stage
{
    public:
        Stage()
        {
            font.loadFromFile("font.ttf");
        }

        enum class SubType { ScrollView, SingleView };
        virtual bool instanceOf(const SubType&) = 0;

        virtual void handle(sf::Event&) = 0;
        virtual void draw(void) = 0;
        virtual void resizeEvent(void) = 0;

    protected:
        sf::Font font;
};
