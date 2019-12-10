#include "Image.h"
#include <iostream>

Image::Image(const std::string &filename)
{
    texture.loadFromFile(filename);
    texture.setSmooth(true);
    sprite.setTexture(texture);
    fitToScreen();
}

sf::Sprite&
Image::getSprite()
{
    return sprite;
}

void
Image::fitToScreen()
{
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    float xScale = (float)videoMode.width / texture.getSize().x;
    float yScale = (float)videoMode.height / texture.getSize().y;

    if (xScale < yScale)
        sprite.setScale(xScale, xScale);
    else
        sprite.setScale(yScale, yScale);

    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    sprite.setPosition(videoMode.width / 2, videoMode.height / 2);
}

void
Image::zoom(sf::Event::MouseWheelScrollEvent& scrollEvent)
{
    if (scrollEvent.delta < 0)
    {
        sprite.scale(0.95f, 0.95f);
    }
    else if (scrollEvent.delta > 0)
    {
        sprite.setOrigin(mousePositionInSprite() / sprite.getScale().x);
        sprite.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        sprite.scale(1.05f, 1.05f);
    }
}

sf::Vector2f
Image::mousePositionInSprite()
{
    return sf::Vector2f(sf::Mouse::getPosition().x - sprite.getGlobalBounds().left,
            sf::Mouse::getPosition().y - sprite.getGlobalBounds().top);
}

sf::Vector2i
Image::mouseVector()
{
    return sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - sf::Mouse::getPosition().x,
            sf::VideoMode::getDesktopMode().height / 2 - sf::Mouse::getPosition().y);
}

void
Image::move(int deltaX, int deltaY)
{
    sprite.move(deltaX, deltaY);
}
