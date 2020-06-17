#include "ImageCache.h"

ImageCache::ImageCache(const char* path)
    : folder(path)
{ 
    //loadImages();
}

void
ImageCache::loadImages()
{
    for (; folder.iterator != folder.iteratorEnd; folder.iterator++)
    {
        images.push_back(ImageData(folder.iterator->string()));
    }
}

void
ImageCache::update()
{
    std::for_each(images.begin(), images.end(), [](ImageData& p) { p.update(); });
}

const sf::Sprite&
ImageCache::next()
{
    images.push_back(ImageData(folder.iterator->string()));
    folder.iterator++;
    return images.back().getSprite();
}

bool
ImageCache::hasNext()
{
    return folder.iterator != folder.iteratorEnd;
}
