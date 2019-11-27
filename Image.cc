#include "Image.h"

Image::Image(const std::string &filename)
{
    texture.loadFromFile(filename);
    texture.setSmooth(true);
}

sf::Sprite&
Image::getSprite()
{
    sprite.setTexture(texture);
    fitToScreen();
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
