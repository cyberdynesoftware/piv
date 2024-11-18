#pragma once

#include "SFML/Graphics.hpp"

class Notification : public sf::Drawable
{
    public:
        Notification(void);

        Notification& setMessage(const std::string& msg);
        Notification& setColor(const sf::Color& color);
        Notification& setSize(unsigned int size);
        Notification& setWidth(float width);
        Notification& setPosition(const sf::Vector2f& position);

        sf::Time timeout;
        sf::Time timeStamp;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Text text;
        sf::RectangleShape background;
};
