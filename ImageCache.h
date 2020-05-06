#pragma once

#include "Image.h"
#include "Folder.h"

class ImageCache
{
    public:
        ImageCache(const char* path);

        Image* getCurrent(void);
        void next(void);
        void previous(void);
        void random(void);

    private:
        Folder folder;
        Image* current;
};
