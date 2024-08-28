#pragma once

#include "SFML/Graphics.hpp"

class TextWithBackground : public sf::Drawable
{
    public:
        enum class Config { info, help, warning, progress };
        TextWithBackground(Config config, unsigned int textSize);

        void setTextAndWidth(std::string msg, float width);
        void setTextAndPadding(std::string, const sf::Vector2f& padding);
        void setPosition(const sf::Vector2f& position);
        void setCenterPosition(const sf::Vector2f& position);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Font font;

        sf::Text text;
        sf::RectangleShape background;
};
