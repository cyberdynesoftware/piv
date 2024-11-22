#include "ProgressBarLabel.h"
#include "FontHolder.h"

ProgressBarLabel::ProgressBarLabel()
{
    text.setFont(FontHolder::font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(15);

    background.setFillColor(sf::Color(255, 255, 255, 128));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.f);
}

void 
ProgressBarLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw(text, states);
}

ProgressBarLabel&
ProgressBarLabel::setMessage(const std::string& msg)
{
    text.setString(msg);
    return *this;
}

ProgressBarLabel&
ProgressBarLabel::setPosition(const sf::Vector2f& position, float progressFraction)
{
    background.setSize(text.getLocalBounds().getSize() + padding);
    auto backBounds = background.getLocalBounds();
    background.setOrigin(backBounds.width, backBounds.height * progressFraction);
    background.setPosition(position);

    auto textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width + padding.x, (textBounds.height + textBounds.top * 2) * progressFraction);
    text.setPosition(position);
    return *this;
}
