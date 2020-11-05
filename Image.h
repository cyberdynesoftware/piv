#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedGIF.h"
#include <future>

class Image
{
    public:
        Image(const std::string&);

        void fitTo(const sf::Vector2u&);
        void square(int);
        void update(void);

        sf::Sprite sprite;
        bool ready = false;
        std::string errormsg;
        std::string path;

    private:
        std::unique_ptr<AnimatedGIF> gif;
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage = false;
        int squareImageEdgeLength;
        bool enframe = false;
        sf::Vector2u frame;
        bool animateImage = false;
        sf::Time delay;

        void init(const std::string&);
};
