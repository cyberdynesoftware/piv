#include "Image.h"
#include <iostream>
#include "Folder.h"

Image::Image(const std::string& path):
    path(path)
{
    future = std::async(std::launch::async, &Image::init, this, path);
}

void
Image::init(const std::string& path)
{
    gif = std::make_unique<AnimatedGIF>(path.c_str());
    if (gif->isGIF())
    {
        gif->update(texture);
        animateImage = true;
        ready = true;
        clock.restart();
    }
    else
    {
        gif.reset();
        ready = texture.loadFromFile(path);
    }

    if (ready)
    {
        texture.setSmooth(true);
        sprite.setTexture(texture, true);

        info = Folder::filename(path);
        info.append("\n").append(std::to_string(sprite.getTexture()->getSize().x));
        info.append("x").append(std::to_string(sprite.getTexture()->getSize().y));
        info.append("\n").append(std::to_string(Folder::fileSize(path) / 1000)).append(" kB");
    }

    if (squareImage) square(squareImageEdgeLength);
    if (enframe) fitTo(frame);
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
    if (!animateImage) return;

    if (clock.getElapsedTime() > gif->delay)
    {
        gif->update(texture);
        clock.restart();
    }
}
