#ifndef __FOLDER_H__
#define __FOLDER_H__

#include "Image.h"

class Folder
{
    public:
        Folder(std::string path);

        Image firstImage(void);
        Image nextImage(void);
        Image prevImage(void);

    private:
        std::string directory;
        std::vector<std::string> filenames;
        std::vector<std::string>::const_iterator imageIter;
        std::vector<std::string> extensions = {"jpg", "jpeg", "gif", "png"};

        bool isDirectory(const std::string &path);
        void scanDirectory(const std::string &dir);
        bool isImage(const std::string &filename);
};

#endif
