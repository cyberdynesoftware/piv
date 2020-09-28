#pragma once

#include <OpenImageIO/imagebuf.h>
#include <SFML/Graphics.hpp>

class Image
{
    public:
        Image();
        ~Image();

        void init(const std::string&);
        void load(void);
        void fitTo(const sf::Vector2u&);
        void update(void);
        void square(void);

        sf::Sprite sprite;
        bool valid;
        std::string errormsg;

    private:
        OIIO::ImageBuf *buffer;
        sf::Texture texture;
        sf::Clock clock;

        void addAlphaChannel(void);
};
