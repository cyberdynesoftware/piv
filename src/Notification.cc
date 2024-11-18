#include "Notification.h"
#include "FontHolder.h"

Notification::Notification()
{
    text.setFont(FontHolder::font);
    background.setFillColor(sf::Color(0, 0, 0, 128));
}

void 
Notification::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw(text, states);
}

Notification&
Notification::setMessage(const std::string& msg)
{
    text.setString(msg);
    return *this;
}

Notification&
Notification::setColor(const sf::Color& color)
{
    text.setFillColor(color);
    return *this;
}

Notification&
Notification::setSize(unsigned int size)
{
    text.setCharacterSize(size);
    return *this;
}

Notification&
Notification::setWidth(float width)
{
    background.setSize(sf::Vector2f(width, text.getLocalBounds().height + 2 * text.getLocalBounds().top));
    return *this;
}

Notification&
Notification::setPosition(const sf::Vector2f& position)
{
    background.setPosition(position);
    text.setPosition(position);
    return *this;
}
