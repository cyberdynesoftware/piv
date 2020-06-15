#include "ImageData.h"

ImageData::ImageData(std::string& path)
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

sf::Sprite&
ImageData::getSprite()
{
    return sprite;
}
