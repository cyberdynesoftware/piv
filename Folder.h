#pragma once

#include <boost/filesystem.hpp>

class Folder
{
    public:
        Folder(const char* arg);

        typedef std::vector<boost::filesystem::path>::const_iterator FolderIter;
        FolderIter cbegin(void);
        FolderIter cend(void);
        FolderIter currentItem;

    private:
        std::vector<boost::filesystem::path> files;
        std::vector<std::string> extensions = {".jpg", ".jpeg", ".gif", ".png"};

        void scanDirectory(const boost::filesystem::path& p);
        bool isImage(const boost::filesystem::path& p);
};
