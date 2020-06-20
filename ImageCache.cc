#include "ImageCache.h"
#include <chrono>

ImageCache::ImageCache(const char* path)
    : folder(path)
{ }

void
ImageCache::loadImages(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        futures.push_back(std::async(std::launch::async, &ImageCache::loadImage, this, folder.iterator->string()));
        folder.iterator++;
        if (folder.iterator == folder.iteratorEnd) break;
    }
}

    ImageData*
ImageCache::loadImage(const std::string& path)
{
    return new ImageData(path);
}

    ImageCache::ImageIter
ImageCache::begin()
{
    for (auto iter = futures.begin(); iter != futures.end(); )
    {
        if (iter->wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready)
        {
            images.push_back(iter->get());
            futures.erase(iter);
        } else {
            iter++;
        }

    }
    return images.begin();
}

    ImageCache::ImageIter
ImageCache::end()
{
    return images.end();
}

void
ImageCache::update()
{
    std::for_each(images.begin(), images.end(), [](ImageData* p) { p->update(); });
}

//const sf::Sprite&
//ImageCache::next()
//{
//    images.push_back(ImageData(folder.iterator->string()));
//    folder.iterator++;
//    return images.back().getSprite();
//}
