#include "ImageData.h"

ImageData::ImageData(const std::string& path)
{
    gif = std::make_unique<AnimatedGIF>(path.c_str());
    if (gif->test())
    {
        gif->load();
        gif->update(sprite);
        isGIF = true;
    }
    else
    {
        texture.loadFromFile(path);
        texture.setSmooth(true);
        sprite.setTexture(texture, true);
        isGIF = false;
    }
}

const sf::Sprite&
ImageData::getSprite() const
{
    return sprite;
}

void
ImageData::update()
{
    if (isGIF) gif->update(sprite);
}
