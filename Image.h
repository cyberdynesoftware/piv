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
        void setPosition(const sf::Vector2f&);

        sf::Sprite sprite;
        bool ready = false;
        bool hasPosition = false;
        sf::Vector2f position;
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
