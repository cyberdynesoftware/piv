#include "Folder.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>

Folder::Folder(std::string path)
{
    if (isDirectory(path)) {
        directory = path;
        scanDirectory(directory);
        imageIter = filenames.cbegin();
    } else {
        int index = path.find_last_of("/");
        directory = path.substr(0, index);
        scanDirectory(directory);
        std::string filename = path.substr(index + 1);
        imageIter = std::find(filenames.cbegin(), filenames.cend(), filename);
    }

}

bool
Folder::isDirectory(const std::string &path)
{
    struct stat _stat;
    stat(path.c_str(), &_stat);
    return S_ISDIR(_stat.st_mode);
}

void
Folder::scanDirectory(const std::string &dir)
{
    DIR *dp;
    if ((dp = opendir(dir.c_str())) == NULL)
    {
        std::cerr << "Error opening directory: " << dir << std::endl;
        exit(1);
    }

    struct dirent* _dirent;
    while ((_dirent = readdir(dp)) != NULL)
        if (isImage(_dirent->d_name))
            filenames.push_back(_dirent->d_name);

    closedir(dp);

    if (filenames.empty())
    {
        std::cerr << dir << " contains no images!" << std::endl;
        exit(1);
    }

    std::sort(filenames.begin(), filenames.end());
}

bool
Folder::isImage(const std::string &filename)
{
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(),
            [](const char c) { return std::tolower(c); });
    return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
}

Image
Folder::firstImage()
{
    return Image(directory + "/" + *imageIter);
}

Image
Folder::nextImage()
{
    if (++imageIter == filenames.cend()) imageIter = filenames.cbegin();
    return Image(directory + "/" + *imageIter);
}

Image
Folder::prevImage()
{
    if (imageIter == filenames.cbegin()) imageIter = filenames.cend();
    return Image(directory + "/" + *--imageIter);
}
