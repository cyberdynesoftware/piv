#pragma once

#include <SFML/Graphics.hpp>

class Icon
{
    public:
        static sf::Image createIcon(void)
        {
            sf::Image icon;
            icon.create(32, 32, sf::Color::Transparent);

            for (int i = 0; i < 28; i++)
            {
                icon.setPixel(i, 4, sf::Color::Black);
                icon.setPixel(i, 31, sf::Color::Black);
                icon.setPixel(0, i + 4, sf::Color::Black);
                icon.setPixel(27, i + 4, sf::Color::Black);
                icon.setPixel(i + 2, 2, sf::Color::Black);
                icon.setPixel(29, i + 2, sf::Color::Black);
                icon.setPixel(i + 4, 0, sf::Color::Black);
                icon.setPixel(31, i, sf::Color::Black);
            }

            return icon;
        };
};
