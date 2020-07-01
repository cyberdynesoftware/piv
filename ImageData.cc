#include "ImageData.h"

ImageData::ImageData(const std::string& path)
{
    gif = std::make_unique<AnimatedGIF>(path.c_str());
    if (gif->test())
    {
        gif->load();
        gif->update(sprite, false);
        gif->update(squareSprite, true);
        isGIF = true;
    }
    else
    {
        texture.loadFromFile(path);
        texture.setSmooth(true);
        sprite.setTexture(texture, true);
        squareSprite.setTexture(texture, true);
        isGIF = false;
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
    if (isGIF)
    {
        gif->update(sprite, false);
        gif->update(squareSprite, true);
    }
}
