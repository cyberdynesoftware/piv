#pragma once

#include "Image.h"
#include "Folder.h"
#include <deque>
#include <vector>

class MultiImageView
{
    public:
        MultiImageView(Folder&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        void resize(bool);

    private:
        Folder& folder;
        Folder::FolderIter folderIter;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::deque<Image*> images;
        bool showInfo = false;
        std::vector<int> columnOffsets;
        int scrollSpeed = 0;
        int bottom = 0;
        float yViewPosition;
        float yViewSize;
        int targetImageWidth;
        sf::Font font;
        Image* selectedImage = NULL;

        void loadImageRow(void);
        void scrollView(int);
        int minColumnIndex(void);
        void layout(Image*);
        void drawInfoBox(Image*);
        bool visible(Image*);
        void selectImage(void);
};
