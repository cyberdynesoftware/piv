#include "Image.h"
#include <iostream>
#include "AnimatedGIF.h"
#include "WebpImage.h"

Image::Image(const std::string& p):
    path(p)
{
    future = std::async(std::launch::async, &Image::init, this, path);
}

Image::~Image()
{
    future.wait();
    if (animatedImage) delete animatedImage;
}

void
Image::init(const std::string& path)
{
    if ((valid = (initIfGIF(path) || initIfWebp(path) || initJPeg(path))))
    {
        info.append(Folder::filename(path));
        info.append("\n").append(std::to_string(sprite.getTexture()->getSize().x));
        info.append("x").append(std::to_string(sprite.getTexture()->getSize().y));
        info.append(", ").append(std::to_string(Folder::fileSize(path) / 1000)).append("kB");
    }

    ready = true;
}

bool
Image::initIfGIF(const std::string& path)
{
    AnimatedGIF* gif = new AnimatedGIF(path.c_str());
    if (gif->isGIF())
    {
        animatedImage = gif;
        gif->load();
        gif->update(sf::milliseconds(0), sprite);
        clock.restart();
        return true;
    }
    else
    {
        delete gif;
        return false;
    }
}

bool
Image::initIfWebp(const std::string& path)
{
    WebpImage* webp = new WebpImage(path.c_str());
    if (webp->isWebp())
    {
        animatedImage = webp;
        webp->prepare(sprite);
        webp->update(sf::milliseconds(0), sprite);
        return true;
    }
    else
    {
        delete webp;
        return false;
    }
}

bool
Image::initJPeg(const std::string& path)
{
    if (texture.loadFromFile(path))
    {
        texture.setSmooth(true);
        sprite.setTexture(texture);
        return true;
    }
    else
    {
        return false;
    }
}

void
Image::update()
{
    if (animatedImage && animatedImage->animate && clock.getElapsedTime() > animatedImage->delay)
    {
        animatedImage->update(clock.getElapsedTime(), sprite);
    }
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
