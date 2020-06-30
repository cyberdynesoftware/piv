#include "Folder.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace boost::filesystem;

Folder::Folder(const char* arg)
{
    path p(arg);

    if (is_directory(status(p)))
    {
        scanDirectory(p);
        currentItem = files.cbegin();
    }
    else if (p.parent_path().empty())
    {
        scanDirectory(current_path());
        currentItem = std::find(files.cbegin(), files.cend(), current_path() / p);
    }
    else
    {
        scanDirectory(p.parent_path());
        currentItem = std::find(files.cbegin(), files.cend(), p);
    }

    srand(time(NULL));
}

void
Folder::scanDirectory(const path& p)
{
    for (directory_iterator dir_iter(p); dir_iter != directory_iterator(); dir_iter++)
        if (isImage(*dir_iter))
            files.push_back(*dir_iter);

    if (files.empty())
    {
        std::cerr << p.string() << " contains no images!" << std::endl;
        exit(1);
    }

    std::sort(files.begin(), files.end());
}

bool
Folder::isImage(const path& p)
{
    std::string extension = p.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
            [](const char c) { return std::tolower(c); });
    return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
}

    Folder::FolderIter
Folder::cbegin()
{
    return files.cbegin();
}

    Folder::FolderIter
Folder::cend()
{
    return files.cend();
}
/*
const std::string&
Folder::getCurrent()
{
    return file_iter->string();
}

void
Folder::next()
{
    if (++file_iter == files.cend())
        file_iter = files.cbegin();
}

void
Folder::previous()
{
    if (file_iter == files.cbegin())
        file_iter = files.cend();
    file_iter--;
}

void
Folder::random()
{
    file_iter = files.cbegin();
    std::advance(file_iter, rand() % files.size());
}

void
Folder::select()
{
    try
    {
        path selected = file_iter->parent_path();
        selected /= "selected";
        create_directory(selected);
        selected /= file_iter->filename();
        copy_file(*file_iter, selected);
    }
    catch (const boost::system::system_error &error)
    {
        if (error.code() != boost::system::errc::file_exists)
            throw;
    }
}

void
Folder::trash()
{
    path selected = file_iter->parent_path();
    selected /= ".trashed";
    create_directory(selected);
    selected /= file_iter->filename();
    rename(*file_iter, selected);
    file_iter = files.erase(file_iter);
}
*/
