#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedImage : public Image
{
    public:
        virtual void load(const sf::Time& time)
        {
            sprite.setTexture(frameIter->texture, false);
            lastFrameUpdate = time;
        }

        virtual void update(const sf::Time& time)
        {
            if (animate && lastFrameUpdate + frameIter->delay < time)
            {
                if (++frameIter == frames.end())
                {
                    frameIter = frames.begin();
                }

                load(time);
            }
        }

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
