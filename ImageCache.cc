#include "ImageCache.h"

ImageCache::ImageCache(const char* path)
    : folder(path)
{ }

Image*
ImageCache::getCurrent()
{
    current = new Image(folder.getCurrent());
    return current;
}

void
ImageCache::next()
{
    folder.next();
}

void
ImageCache::previous()
{
    folder.previous();
}

void
ImageCache::random()
{
    folder.random();
}
