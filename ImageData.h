#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedGIF.h"

class ImageData
{
    public:
        ImageData(const std::string&);

        const sf::Sprite& getSprite(void) const;
        void update(void);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
        bool isGIF;
        std::unique_ptr<AnimatedGIF> gif;
};
