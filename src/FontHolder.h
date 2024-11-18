#pragma once

#include "SFML/Graphics.hpp"

class FontHolder
{
    public:
        static sf::Font font;

        static void init(void);
};
