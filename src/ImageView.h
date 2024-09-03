#pragma once

#include <deque>
#include "SFML/Window.hpp"

class ImageView
{
    public:
        virtual void process(const sf::Event& event) = 0;

        std::deque<Image*>::iterator imageIter;
};
