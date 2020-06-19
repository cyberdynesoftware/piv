#pragma once

#include "Folder.h"
#include "ImageData.h"
#include <future>

class ImageCache
{
    public:
        ImageCache(const char* path);

        void loadImages(int);
        void update(void);

        typedef std::vector<ImageData*>::const_iterator ImageIter;
        ImageIter cbegin(void);
        ImageIter cend(void);

    private:
        Folder folder;
        std::vector<ImageData*> images;
        std::vector<std::future<ImageData*>> futures;

        ImageData* loadImage(const std::string&);
};
