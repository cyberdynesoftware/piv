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

    private:
        OIIO::ImageBuf buffer;
        sf::Texture texture;
        sf::Clock clock;
        std::future<void> future;
        bool squareImage;
        bool enframe;
        sf::Vector2u frame;

        void init(const std::string&);
        void load(void);
        void addAlphaChannel(void);
        void square(void);
};
