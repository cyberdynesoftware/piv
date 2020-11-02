#pragma once

#include <OpenImageIO/imagebuf.h>
#include <SFML/Graphics.hpp>

class Image
{
    public:
        Image(const std::string&, bool);

        void fitTo(const sf::Vector2u&);
        void update(void);

        sf::Sprite sprite;
        bool valid = false;
        std::string errormsg;
        std::string path;

    private:
        OIIO::ImageBuf buffer;
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage;
        bool enframe;
        sf::Vector2u frame;
        bool animateImage;
        sf::Time delay;

        void init(const std::string&);
        void readPixels(void);
        OIIO::ImageBuf fixChannels(OIIO::ImageBuf&);
        void square(void);
};
