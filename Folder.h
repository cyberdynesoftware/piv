#pragma once

#include <vector>
#include <string>

class Folder
{
    public:
        Folder(const char* arg);

        typedef std::vector<std::string>::const_iterator FolderIter;
        FolderIter cbegin(void);
        FolderIter cend(void);
        FolderIter currentItem;

        int size(void);
        static std::string filename(const std::string&);
        static int fileSize(const std::string&);

    private:
        std::vector<std::string> files;

        void scanDirectory(const std::string& p);
};
