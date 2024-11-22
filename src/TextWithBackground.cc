#include "TextWithBackground.h"
#include "FontHolder.h"

TextWithBackground::TextWithBackground(Config config, unsigned int textSize)
{
    text.setFont(FontHolder::font);
    text.setCharacterSize(textSize);

    switch (config)
    {
        case Config::info:
            text.setFillColor(sf::Color::White);
            background.setFillColor(sf::Color(0, 0, 0, 64));
            break;
        case Config::progress:
            text.setFillColor(sf::Color::Black);
            background.setFillColor(sf::Color(255, 255, 255, 128));
            background.setOutlineColor(sf::Color::Black);
            background.setOutlineThickness(1.f);
            break;
        case Config::help:
            text.setFillColor(sf::Color::White);
            background.setFillColor(sf::Color(0, 0, 0, 192));
            break;
        case Config::warning:
            text.setFillColor(sf::Color::Red);
            background.setFillColor(sf::Color(0, 0, 0, 192));
            break;
    }
}

void 
TextWithBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw(text, states);
}

void
TextWithBackground::setTextAndWidth(std::string msg, float width)
{
    text.setString(msg);
    background.setSize(sf::Vector2f(width, text.getLocalBounds().top * 2 + text.getLocalBounds().height));
}

void
TextWithBackground::setTextAndPadding(std::string msg, const sf::Vector2f& padding)
{
    text.setString(msg);
    background.setSize(text.getLocalBounds().getSize() + padding);
}

void
TextWithBackground::setPosition(const sf::Vector2f& position)
{
    background.setPosition(position);
    text.setPosition(position);
}

void
TextWithBackground::setCenterPosition(const sf::Vector2f& position)
{
    background.setOrigin(background.getLocalBounds().getSize() / 2.f);
    background.setPosition(position);

    text.setOrigin(text.getLocalBounds().getSize() / 2.f);
    text.setPosition(position - text.getLocalBounds().getPosition());
}
