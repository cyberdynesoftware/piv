#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedGIF.h"
#include "WebpImage.h"
#include <future>

class Image
{
    public:
        Image(const std::string&);
        ~Image();

        void fitTo(const sf::Vector2u&);
        void square(int);
        void update(void);

        sf::Sprite sprite;
        bool ready = false;
        std::string errormsg;
        std::string path;

    private:
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage = false;
        int squareImageEdgeLength;
        bool enframe = false;
        sf::Vector2u frame;
        AnimatedImage* animatedImage = NULL;
        sf::Time delay;

        void init(const std::string&);
        bool initIfGIF(const std::string&);
        bool initIfWebp(const std::string&);
};
