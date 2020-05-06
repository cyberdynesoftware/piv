#pragma once

#include <boost/filesystem.hpp>

class Folder
{
    public:
        Folder(const char* arg);

        const std::string& getCurrent(void);
        void next(void);
        void previous(void);
        void random(void);
        void select(void);
        void trash(void);

    private:
        std::vector<boost::filesystem::path> files;
        std::vector<boost::filesystem::path>::const_iterator file_iter;
        std::vector<std::string> extensions = {".jpg", ".jpeg", ".gif", ".png"};

        void scanDirectory(const boost::filesystem::path& p);
        bool isImage(const boost::filesystem::path& p);
};
