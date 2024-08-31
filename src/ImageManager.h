#pragma once

#include "Folder.h"
#include "Image.h"
#include <deque>

class ImageManager
{
    public:
        ImageManager(Folder& folder);

        void loadImages(int number);
        void update(void);

        void copySelectedImages(void);
        void moveSelectedImages(void);

        int numberOfFiles(void);
        
        std::deque<Image*> images;

    private:
        Folder& folder;
        Folder::iter folderIter;

        std::deque<Image*> imagesLoading;
};
