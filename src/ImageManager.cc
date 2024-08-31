#include "ImageManager.h"

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
            imagesLoading.push_back(new Image(*folderIter++));
        }
    }
}

void
ImageManager::update()
{
    for (auto image: imagesLoading)
    {
        if (image->ready)
        {
            if (image->valid)
            {
                images.push_back(image);
            }
            imagesLoading.pop_front();
        }
        else
        {
            break;
        }
    }
}

void
ImageManager::copySelectedImages()
{
    for (auto image : images)
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
    std::deque<Image*> temp;

    for (auto image : images)
    {
        if (image->selected)
        {
            folder.moveToSelection(image->path);
            delete image;
        }
        else
        {
            temp.push_back(image);
        }
    }

    folder.scan();
    images = temp;
    folderIter = std::find(folder.cbegin(), folder.cend(), images.back()->path);
    folderIter++;
}

int
ImageManager::numberOfFiles()
{
    return folder.size();
}
