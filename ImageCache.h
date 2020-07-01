#pragma once

#include "Folder.h"
#include "ImageData.h"
#include <future>
#include <queue>

class ImageCache
{
    public:
        ImageCache(const char* path);

        void loadImages(int);
        void update(void);

        typedef std::list<ImageData*>::iterator ImageIter;
        ImageIter begin(void);
        ImageIter end(void);
        ImageIter currentImage;

    private:
        Folder folder;
        std::list<ImageData*> images;
        std::queue<std::future<ImageData*>> futures;

        ImageData* loadImage(const std::string&);
};
