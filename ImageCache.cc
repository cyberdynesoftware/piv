#include "ImageCache.h"

ImageCache::ImageCache(const char* path)
    : folder(path)
{ 
    load();
}

void
ImageCache::load()
{
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
}
