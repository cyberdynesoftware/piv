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
        bool loadingComplete(void);

        typedef std::vector<ImageData*>::iterator ImageIter;
        ImageIter begin(void);
        ImageIter end(void);

    private:
        Folder folder;
        std::vector<ImageData*> images;
        std::vector<std::future<ImageData*>> futures;

        ImageData* loadImage(const std::string&);
};
