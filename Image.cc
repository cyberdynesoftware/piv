#include "Image.h"
#include <iostream>
#include "Folder.h"

Image::Image(const std::string& path):
    path(path)
{
    future = std::async(std::launch::async, &Image::init, this, path);
}

Image::~Image()
{
    if (animatedImage) delete animatedImage;
}

void
Image::init(const std::string& path)
{
    if ((ready = initIfGIF(path) || initIfWebp(path) || texture.loadFromFile(path)))
    {
        texture.setSmooth(true);
        sprite.setTexture(texture, true);

        info.append("\n\"").append(Folder::filename(path)).append("\"");
        info.append("\n").append(std::to_string(sprite.getTexture()->getSize().x));
        info.append("x").append(std::to_string(sprite.getTexture()->getSize().y));

        if (squareImage) square(squareImageEdgeLength);
        if (enframe) fitTo(frame);
    }
    else
    {
        info.append("\nError loading \"").append(Folder::filename(path)).append("\"");
    }
    info.append("\n").append(std::to_string(Folder::fileSize(path) / 1000)).append(" kB");
}

bool
Image::initIfGIF(const std::string& path)
{
    AnimatedGIF* gif = new AnimatedGIF(path.c_str());
    if (gif->isGIF())
    {
        animatedImage = gif;
        gif->update(texture);
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
        std::cout << "sí" << std::endl;
    }
    else
    {
        delete webp;
    }

    return false;
}

void
Image::fitTo(const sf::Vector2u& window)
{
    enframe = true;
    frame = window;

    if (ready)
    {
        const sf::Vector2u& size = sprite.getTexture()->getSize();

        float xScale = (float)window.x / size.x;
        float yScale = (float)window.y / size.y;
        float scale = (xScale < yScale) ? xScale : yScale;
        sprite.setScale(scale, scale);

        sprite.setOrigin(size.x / 2, size.y / 2);
        sprite.setPosition(window.x / 2, window.y / 2);

        enframe = false;
    }
}

void
Image::square(int targetSize)
{
    squareImage = true;
    squareImageEdgeLength = targetSize;

    if (ready)
    {
        auto size = sprite.getTexture()->getSize();
        auto offset = std::abs((int)size.x - (int)size.y) / 2;

        if (size.x < size.y)
            sprite.setTextureRect(sf::IntRect(0, offset, size.x, size.x));
        else
            sprite.setTextureRect(sf::IntRect(offset, 0, size.y, size.y));
        
        float factor = 1.f * targetSize / sprite.getLocalBounds().width;
        sprite.setScale(factor, factor);
    }
}

void
Image::update()
{
    if (animatedImage && animatedImage->animate && clock.getElapsedTime() > animatedImage->delay)
    {
        animatedImage->update(texture);
        clock.restart();
    }
}
