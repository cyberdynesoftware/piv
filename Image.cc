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
    {
        sprite.setScale(xScale, xScale);
        float offset = (float)videoMode.height - texture.getSize().y * xScale;
        sprite.setPosition(offset / 2, 0);
    }
    else
    {
        sprite.setScale(yScale, yScale);
        float offset = (float)videoMode.width - texture.getSize().x * yScale;
        sprite.setPosition(offset / 2, 0);
    }
}

void
Image::zoom(sf::Event::MouseWheelScrollEvent& scrollEvent)
{
    if (scrollEvent.delta < 0)
    {
        sprite.scale(0.9f, 0.9f);
    }
    else if (scrollEvent.delta > 0)
    {
        sprite.scale(1.1f, 1.1f);
    }
}
