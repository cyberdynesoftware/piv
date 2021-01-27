#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Folder.h"

class ImageView
{
    public:
        ImageView()
        {
            if (Folder::fileExists("/System/Library/Fonts/Supplemental/Arial.ttf"))
                font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
            else if (Folder::fileExists("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
                font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
            else
                std::cerr << "Warning: Could not find a font to load." << std::endl;
        }

        virtual void handle(sf::Event&) = 0;
        virtual void draw(void) = 0;

    protected:
        sf::Font font;
};
