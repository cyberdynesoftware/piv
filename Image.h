#pragma once

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <SFML/Graphics.hpp>

class Image
{
    public:
        void load(const std::string&);
        void fitTo(const sf::Vector2u&);

        sf::Texture texture;
        sf::Sprite sprite;
        bool valid;
        std::string errormsg;
};
