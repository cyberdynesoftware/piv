#pragma once

#include "Folder.h"
#include "Image.h"
#include "SFML/System.hpp"
#include <deque>

class ImageManager
{
    public:
        ImageManager(Folder& folder);

        void loadImages(int number);
        void update(const sf::Time& time);

        void copySelectedImages(void);
        void moveSelectedImages(void);

        int numberOfFiles(void);
        
        std::deque<std::unique_ptr<Image>> images;

        enum SORT_ORDER { NAME, RANDOM, OLDEST, NEWEST, ENUM_GUARD };
        void changeSortOrder(SORT_ORDER sortOrder);

    private:
        Folder& folder;
        Folder::iter folderIter;

        std::deque<std::future<std::unique_ptr<Image>>> imagesLoading;

        std::unique_ptr<Image> loadImage(const std::string& path);
};
