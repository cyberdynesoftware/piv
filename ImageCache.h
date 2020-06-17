#pragma once

#include "Folder.h"
#include "ImageData.h"

class ImageCache
{
    public:
        ImageCache(const char* path);

        const sf::Sprite& next(void);
        bool hasNext(void);
        void update(void);

        std::vector<ImageData>::const_iterator cbegin(void) { return images.cbegin(); }
        std::vector<ImageData>::const_iterator cend(void) { return images.cend(); }

    private:
        Folder folder;
        std::vector<ImageData> images;

        void loadImages(void);
};
