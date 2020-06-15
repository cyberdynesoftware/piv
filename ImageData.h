#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedGIF.h"

class ImageData
{
    public:
        ImageData(std::string&);

        sf::Sprite& getSprite(void);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
        bool isGIF;
        std::unique_ptr<AnimatedGIF> gif;
};
