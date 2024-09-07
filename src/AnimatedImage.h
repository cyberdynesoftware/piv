#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedImage
{
    protected:
        bool animate;
        sf::Time lastFrameUpdate;

        struct Frame 
        {
            sf::Time delay;
            sf::Texture texture;
        };

        std::vector<Frame> frames;
        std::vector<Frame>::iterator frameIter;
};
