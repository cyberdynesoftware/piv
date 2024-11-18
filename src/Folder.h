#pragma once

#include <vector>
#include <string>

class Folder
{
    public:
        Folder(const char* arg);

        typedef std::vector<std::string>::const_iterator iter;
        iter cbegin(void);
        iter cend(void);

        int size(void);
        bool selectedFolderExistsNotEmpty(void) const;
        void copyToSelection(const std::string&);
        void moveToSelection(const std::string&);
        void scan(void);
        void sortCaseInsensitive(void);
        void sortByModTime(bool);
        void shuffle(void);

        static std::string filename(const std::string&);
        static int fileSize(const std::string&);
        static bool fileExists(const std::string&);

    private:
        std::string folderPath;
        std::string selectedFolder;
        std::vector<std::string> files;
};
