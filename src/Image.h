#pragma once

#include <SFML/Graphics.hpp>
#include <future>
#include "AnimatedImage.h"
#include "Folder.h"

class Image
{
    public:
        Image(const std::string&);
        ~Image();

        void update(void);
        void setPosition(const sf::Vector2f&);
        void centerOrigin(void);
        void fitTo(const sf::View&);

        sf::Sprite sprite;
        bool valid;
        bool ready = false;
        bool hasPosition = false;
        sf::Vector2f position;
        std::string info;
        std::string path;
        bool selected = false;

    private:
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        AnimatedImage* animatedImage = nullptr;
        sf::Time delay;

        void init(const std::string&);
        bool initIfGIF(const std::string&);
        bool initIfWebp(const std::string&);
        bool initJPeg(const std::string&);
};
