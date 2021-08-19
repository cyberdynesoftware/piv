#pragma once

#include "ImageView.h"
#include "Image.h"
#include "Folder.h"
#include <deque>
#include <vector>

class MultiImageView : public ImageView
{
    public:
        MultiImageView(Folder&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        void resize(void);

    private:
        Folder& folder;
        Folder::FolderIter folderIter;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::deque<Image*> images;
        bool showInfo = false;
        std::vector<unsigned int> columnOffsets;
        int scrollSpeed = 0;
        float viewPosition;
        int targetImageWidth;

        void loadImageRow(void);
        void scroll(int);
        float viewBottom(void);
};
