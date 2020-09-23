#pragma once

#include <vector>
#include <string>
#include <OpenImageIO/imagecache.h>

class Folder
{
    public:
        Folder(const char* arg);

        typedef std::vector<std::string>::const_iterator FolderIter;
        FolderIter cbegin(void);
        FolderIter cend(void);
        FolderIter currentItem;

        OIIO::ImageCache* imageCache;

    private:
        std::vector<std::string> files;

        void scanDirectory(const std::string& p);
};
