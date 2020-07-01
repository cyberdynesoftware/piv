#include "ImageCache.h"
#include <chrono>

ImageCache::ImageCache(const char* path)
    : folder(path)
{ }

void
ImageCache::loadImages(int amount)
{
    if (futures.empty())
        for (int i = 0; i < amount; i++)
        {
            if (folder.currentItem == folder.cend()) break;
            futures.push(std::async(std::launch::async, &ImageCache::loadImage, this, folder.currentItem->string()));
            folder.currentItem++;
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

    while (!futures.empty())
    {
        if (futures.front().wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready)
        {
            images.push_back(futures.front().get());
            futures.pop();
        }
        else break;
    }
}
