#include "Image.h"
#include <iostream>
#include <format>

void
Image::init(const std::string& p)
{
    path = p;
    valid = texture.loadFromFile(path);
}

void
Image::prepare()
{
    if (valid)
    {
        texture.setSmooth(true);
        sprite.setTexture(texture);
        prepareInfo("stb");
    }
}

void
Image::prepareInfo(const std::string& decoder)
{
    info = std::format("{} ({})\n{}x{}, {}kB",
            Folder::filename(path),
            decoder,
            sprite.getTexture()->getSize().x,
            sprite.getTexture()->getSize().y,
            Folder::fileSize(path) / 1000);
}

void
Image::update(const sf::Time& time)
{
    /*
    if (animatedImage && animatedImage->animate && clock.getElapsedTime() > animatedImage->delay)
    {
        animatedImage->update(clock.getElapsedTime(), sprite);
    }
    */
}

void
Image::setPosition(const sf::Vector2f& p)
{
    position = p;
    sprite.setPosition(p);
    hasPosition = true;
}

void
Image::fitTo(const sf::View& view)
{
    const auto& size = sprite.getTexture()->getSize();

    float xScale = (float) view.getSize().x / size.x;
    float yScale = (float) view.getSize().y / size.y;
    float scale = (xScale < yScale) ? xScale : yScale;
    scale *= .98f;
    sprite.setScale(scale, scale);

    sprite.setOrigin(size.x / 2, size.y / 2);
    sprite.setPosition(view.getCenter());
}

void
Image::scaleTo(int targetWidth)
{
    scale = (float) targetWidth / sprite.getTexture()->getSize().x;
    sprite.setScale(scale, scale);
}

void
Image::resetPositionAndScale()
{
    sprite.setScale(scale, scale);
    sprite.setOrigin(0, 0);
    sprite.setPosition(position);
}
