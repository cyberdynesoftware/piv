#include "ImageCache.h"

ImageCache::ImageCache(const char* path)
    : folder(path)
{ 
    load();
}

sf::Sprite&
ImageCache::getSprite()
{
    return sprite;
}

void
ImageCache::load()
{
    gif = std::make_unique<AnimatedGIF>(folder.getCurrent().c_str());
    if (gif->test())
    {
        gif->load();
        gif->update(sprite);
        currentIsGIF = true;
    }
    else
    {
        texture.loadFromFile(folder.getCurrent());
        texture.setSmooth(true);
        sprite.setTexture(texture, true);
        currentIsGIF = false;
    }
}

void
ImageCache::next()
{
    folder.next();
    load();
}

void
ImageCache::previous()
{
    folder.previous();
    load();
}

void
ImageCache::random()
{
    folder.random();
    load();
}

void
ImageCache::update()
{
    if (currentIsGIF)
        gif->update(sprite);
}
