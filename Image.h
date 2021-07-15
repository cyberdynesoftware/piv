#pragma once

#include <SFML/Graphics.hpp>
#include <future>
#include "AnimatedImage.h"

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
        std::string path;
        std::string info;

    private:
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage = false;
        int squareImageEdgeLength;
        bool enframe = false;
        sf::Vector2u frame;
        AnimatedImage* animatedImage = nullptr;
        sf::Time delay;

        void init(const std::string&);
        bool initIfGIF(const std::string&);
        bool initIfWebp(const std::string&);
        bool initJPeg(const std::string&);
};
