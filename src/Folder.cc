#include "Folder.h"
#include <iostream>
#include <algorithm>
#include <random>
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
    }
    else if (p.parent_path().empty())
    {
        folderPath = current_path().string();
    }
    else
    {
        folderPath = p.parent_path().string();
    }

    scan();
    selectedFolder = path(folderPath).append("piv-selected").string();
}

void
Folder::scan()
{
    path p(folderPath);
    files.clear();

    for (directory_iterator dir_iter(p); dir_iter != directory_iterator(); dir_iter++)
    {
        if (!is_directory(dir_iter->path()))
        {
            files.push_back(dir_iter->path().string());
        }
    }

    if (files.empty())
    {
        std::cerr << p.string() << " contains no images!" << std::endl;
        exit(1);
    }

    sortCaseInsensitive();
}

void
Folder::sortCaseInsensitive()
{
    sort(files.begin(), files.end(), [] (const std::string& lhs, const std::string& rhs)
            { return lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
                    [] (const char& lhs, const char& rhs) { return tolower(lhs) < tolower(rhs); });
            }
        );
}

void
Folder::sortByModTime(bool desc)
{
    sort(files.begin(), files.end(), [desc] (const std::string& lhs, const std::string& rhs)
            { return (last_write_time(path(lhs)) < last_write_time(path(rhs)) != desc); }
        );
}

void
Folder::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::ranges::shuffle(files, g);
}

bool
Folder::selectedFolderExistsNotEmpty() const
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
Folder::copyToSelection(const std::string& item)
{
    if (!exists(selectedFolder))
        create_directory(selectedFolder);

    std::error_code ec;
    copy_file(path(item), path(selectedFolder) / path(item).filename(), ec);
}

void
Folder::moveToSelection(const std::string& item)
{
    if (!exists(selectedFolder))
        create_directory(selectedFolder);

    std::error_code ec;
    rename(path(item), path(selectedFolder) / path(item).filename(), ec);
}
