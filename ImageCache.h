#pragma once

#include "Folder.h"
#include "ImageData.h"

class ImageCache
{
    public:
        ImageCache(const char* path);

        void next(void);
        void previous(void);
        void random(void);
        void update(void);

    private:
        Folder folder;
        std::vector<ImageData> images;

        void load(void);
};
