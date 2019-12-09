#include "Image.h"
#include <iostream>

Image::Image(const std::string &filename)
{
    texture.loadFromFile(filename);
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
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

    sprite.setPosition(videoMode.width / 2, videoMode.height / 2);
}

void
Image::zoom(sf::Event::MouseWheelScrollEvent& scrollEvent)
{
    if (scrollEvent.delta < 0)
        sprite.scale(0.95f, 0.95f);
    else if (scrollEvent.delta > 0)
        sprite.scale(1.05f, 1.05f);
}
