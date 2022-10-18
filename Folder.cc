#include "Folder.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <filesystem>

using namespace std::filesystem;

Folder::Folder(const char* arg)
{
    path p(arg);

    if (is_directory(status(p)))
    {
        folderPath = p.string();
        scan();
        currentItem = files.cbegin();
    }
    else if (p.parent_path().empty())
    {
        folderPath = current_path().string();
        scan();
        currentItem = std::find(files.cbegin(), files.cend(), (current_path() / p).string());
        imageSelected = true;
    }
    else
    {
        folderPath = p.parent_path().string();
        scan();
        currentItem = std::find(files.cbegin(), files.cend(), p.string());
        imageSelected = true;
    }

    selectedFolder = path(folderPath).append("piv-selected").string();
    srand(time(NULL));
}

void
Folder::scan()
{
    path p(folderPath);
    files.clear();

    for (directory_iterator dir_iter(p); dir_iter != directory_iterator(); dir_iter++)
        if (!is_directory(dir_iter->path()))
            files.push_back(dir_iter->path().string());

    if (files.empty())
    {
        std::cerr << p.string() << " contains no images!" << std::endl;
        exit(1);
    }

    //std::sort(files.begin(), files.end());
    sortCaseInsensitive();
}

void
Folder::sortCaseInsensitive()
{
    sort(files.begin(), files.end(), [] (const std::string& lhs, const std::string& rhs)
            {
            return lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
                    [] (const char& lhs, const char& rhs) 
                    { return tolower(lhs) < tolower(rhs); } );
            }
        );
}

bool
Folder::selectedFolderExistsNotEmpty()
{
    return exists(path(selectedFolder)) && !is_empty(path(selectedFolder));
}

Folder::iter
Folder::cbegin()
{
    return files.cbegin();
}

Folder::iter
Folder::cend()
{
    return files.cend();
}

int
Folder::size()
{
    return files.size();
}

int
Folder::fileSize(const std::string& p)
{
    std::error_code ec;
    return file_size(path(p), ec);
}

std::string
Folder::filename(const std::string& path)
{
    return path.substr(path.find_last_of('/') + 1);
}

bool
Folder::fileExists(const std::string& p)
{
    return exists(path(p));
}

void
Folder::copyToSelection(std::string& item)
{
    if (!exists(selectedFolder))
        create_directory(selectedFolder);

    std::error_code ec;
    copy_file(path(item), path(selectedFolder) / path(item).filename(), ec);
}

void
Folder::moveToSelection(std::string& item)
{
    if (!exists(selectedFolder))
        create_directory(selectedFolder);

    std::error_code ec;
    rename(path(item), path(selectedFolder) / path(item).filename(), ec);
}
