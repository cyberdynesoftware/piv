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
