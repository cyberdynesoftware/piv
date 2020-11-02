#pragma once

#include <OpenImageIO/imagebuf.h>
#include <SFML/Graphics.hpp>

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
        OIIO::ImageBuf buffer;
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage = false;
        int imageSize;
        bool enframe = false;
        sf::Vector2u frame;
        bool animateImage;
        sf::Time delay;

        void init(const std::string&);
        void readPixels(void);
        OIIO::ImageBuf fixChannels(OIIO::ImageBuf&);
        void square(void);
};
