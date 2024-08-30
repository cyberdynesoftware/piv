#pragma once

class ImageView
{
    public:
        virtual void process(const sf::Event& event) = 0;
};
