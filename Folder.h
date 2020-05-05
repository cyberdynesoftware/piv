#pragma once

#include "Image.h"
#include <boost/filesystem.hpp>

class Folder
{
    public:
        Folder(char* arg);
        ~Folder();

        Image* getImage(void);
        void next(void);
        void previous(void);
        void random(void);
        void select(void);
        void trash(void);

    private:
        Image* current;
        std::vector<boost::filesystem::path> files;
        std::vector<boost::filesystem::path>::const_iterator file_iter;
        std::vector<std::string> extensions = {".jpg", ".jpeg", ".gif", ".png"};

        void scanDirectory(const boost::filesystem::path& p);
        bool isImage(const boost::filesystem::path& p);
        void init(void);
};
