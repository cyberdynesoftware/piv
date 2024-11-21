#include "Dialog.h"
#include "FontHolder.h"

Dialog::Dialog()
{
    text.setFont(FontHolder::font);
    background.setFillColor(sf::Color(0, 0, 0, 128));
}

void 
Dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw(text, states);
}

Dialog&
Dialog::setMessage(const std::string& msg)
{
    text.setString(msg);
    return *this;
}

Dialog&
Dialog::setColor(const sf::Color& color)
{
    text.setFillColor(color);
    return *this;
}

Dialog&
Dialog::setSize(unsigned int size)
{
    text.setCharacterSize(size);
    return *this;
}

Dialog&
Dialog::setPadding(const sf::Vector2f& padding)
{
    background.setSize(text.getLocalBounds().getSize() + padding);
    return *this;
}

Dialog&
Dialog::setCenter(const sf::Vector2f& center)
{
    background.setOrigin(background.getLocalBounds().getSize() / 2.f);
    background.setPosition(center);
    text.setOrigin(text.getLocalBounds().getSize() / 2.f);
    text.setPosition(center);
    return *this;
}
