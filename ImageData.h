#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedGIF.h"

class ImageData
{
    public:
        ImageData(const std::string&);

        sf::Sprite& getSprite(void);
        sf::Sprite& getSquareSprite(void);
        void update(void);
        bool isValid(void);

        static void square(sf::Sprite&);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Sprite squareSprite;
        bool valid;
        std::unique_ptr<AnimatedGIF> gif;
};
