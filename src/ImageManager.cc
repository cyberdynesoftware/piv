#include "ImageManager.h"
#include "WebpImage.h"
#include "AnimatedGIF.h"
#include <chrono>

using namespace std::chrono_literals;

ImageManager::ImageManager(Folder& folder)
    :
        folder(folder)
{
    folderIter = folder.cbegin();
}

void
ImageManager::loadImages(int number)
{
    if (imagesLoading.empty())
    {
        for (int i = 0; i < number; i++)
        {
            if (folderIter == folder.cend()) break;
            imagesLoading.push_back(std::async(std::launch::async, &ImageManager::loadImage, this, *folderIter++));
        }
    }
}

    std::unique_ptr<Image>
ImageManager::loadImage(const std::string& path)
{
    auto webp = std::make_unique<WebpImage>();
    webp->init(path.c_str());
    if (webp->isWebp())
    {
        webp->prepare();
        return webp;
    }

    auto gif = std::make_unique<AnimatedGIF>();
    gif->init(path.c_str());
    if (gif->isGIF())
    {
        gif->prepare();
        return gif;
    }

    auto image = std::make_unique<Image>();
    image->init(path.c_str());
    image->prepare();
    return image;
}

void
ImageManager::update(const sf::Time& time)
{
    for (auto& future: imagesLoading)
    {
        if (future.wait_for(0s) == std::future_status::ready)
        {
            auto image = future.get();
            if (image->valid)
            {
                image->load(time);
                images.push_back(std::move(image));
            }
            imagesLoading.pop_front();
        }
        else
        {
            break;
        }
    }

    for (auto& image: images)
    {
        image->update(time);
    }
}

void
ImageManager::copySelectedImages()
{
    for (auto& image : images)
    {
        if (image->selected)
        {
            folder.copyToSelection(image->path);
        }
    }
}

void
ImageManager::moveSelectedImages()
{
    std::deque<std::unique_ptr<Image>> temp;

    for (auto& image : images)
    {
        if (image->selected)
        {
            folder.moveToSelection(image->path);
        }
        else
        {
            temp.push_back(std::move(image));
        }
    }

    folder.scan();
    images = std::move(temp);
    folderIter = std::find(folder.cbegin(), folder.cend(), images.back()->path);
    folderIter++;
}

int
ImageManager::numberOfFiles()
{
    return folder.size();
}

void
ImageManager::changeSortOrder(SORT_ORDER sortOrder)
{
    switch (sortOrder)
    {
        case NAME:
            folder.sortCaseInsensitive();
            break;
        case RANDOM:
            break;
        case OLDEST:
            folder.sortByModTime(true);
            break;
        case NEWEST:
            folder.sortByModTime(false);
            break;
    }

    images.clear();
    imagesLoading.clear();
    folderIter = folder.cbegin();
}
