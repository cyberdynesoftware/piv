#pragma once

#include "SFML/Graphics.hpp"

class ProgressBarLabel : public sf::Drawable
{
    public:
        ProgressBarLabel(void);

        ProgressBarLabel& setMessage(const std::string& msg);
        ProgressBarLabel& setPosition(const sf::Vector2f& position, float progressFaction);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Text text;
        sf::RectangleShape background;
        sf::Vector2f padding = sf::Vector2f(10.f, 10.f);
};
