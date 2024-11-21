#pragma once

#include "SFML/Graphics.hpp"

class Dialog : public sf::Drawable
{
    public:
        Dialog(void);

        Dialog& setMessage(const std::string& msg);
        Dialog& setColor(const sf::Color& color);
        Dialog& setSize(unsigned int size);
        Dialog& setPadding(const sf::Vector2f& padding);
        Dialog& setCenter(const sf::Vector2f& center);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Text text;
        sf::RectangleShape background;
};
