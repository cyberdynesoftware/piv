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
        
        std::deque<Image*> images;
        std::deque<Image*>::iterator imageIter;

    private:
        Folder& folder;
        Folder::iter folderIter;

        std::deque<Image*> imagesLoading;
};
