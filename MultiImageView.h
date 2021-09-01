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
        void resize(void);

    private:
        Folder& folder;
        Folder::iter folderIter;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::deque<Image*> images;
        bool showInfo = false;
        std::vector<int> columnOffsets;
        int scrollSpeed = 0;
        int bottom = 0;
        float viewPosition;
        float viewHeight;
        int targetImageWidth;
        sf::Font font;
        Image* elevatedImage = NULL;
        int progressBarWidth = 10;
        sf::RectangleShape progressBar;
        Image* lastVisibleImage;
        bool showSelection = false;

        void loadImageRow(void);
        void scrollView();
        void restrict(sf::View&);
        int minColumnIndex(void);
        void layout(Image*);
        void drawInfoBox(Image*);
        bool isVisible(Image*);
        void pickImage(void);
        void unpickImage(void);
        void relayoutImages(int);
        void drawProgressBar(void);
};
