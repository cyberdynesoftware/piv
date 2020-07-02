#include "ImageData.h"
#include <iostream>

ImageData::ImageData(const std::string& path)
{
    if (AnimatedGIF::test(path.c_str()))
    {
        gif = std::make_unique<AnimatedGIF>();
        gif->load(path.c_str());
        gif->update(sprite, false);
        gif->update(squareSprite, true);
        valid = true;
    }
    else
    {
        valid = texture.loadFromFile(path);
        texture.setSmooth(true);
        sprite.setTexture(texture, true);
        squareSprite.setTexture(texture, true);
        square(squareSprite);
    }
}

sf::Sprite&
ImageData::getSprite()
{
    return sprite;
}

sf::Sprite&
ImageData::getSquareSprite()
{
    return squareSprite;
}

void
ImageData::update()
{
    if (gif != nullptr)
    {
        gif->update(sprite, false);
        gif->update(squareSprite, true);
    }
}

bool
ImageData::isValid()
{
    return valid;
}

void
ImageData::square(sf::Sprite& sprite)
{
    auto size = sprite.getTexture()->getSize();
    auto offset = std::abs((int)size.x - (int)size.y) / 2;

    if (size.x < size.y)
        sprite.setTextureRect(sf::IntRect(0, offset, size.x, size.x));
    else
        sprite.setTextureRect(sf::IntRect(offset, 0, size.y, size.y));
}
