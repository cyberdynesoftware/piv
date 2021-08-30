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
        float yViewPosition;
        float yViewSize;
        int targetImageWidth;
        sf::Font font;
        Image* selectedImage = NULL;
        int progressBarWidth = 10;
        sf::RectangleShape progressBar;
        Image* lastVisibleImage;
        enum scrollStates { NONE, UP, UP_FAST, DOWN, DOWN_FAST };
        scrollStates scrollState = NONE;

        void loadImageRow(void);
        void scrollView();
        void restrict(sf::View&);
        int minColumnIndex(void);
        void layout(Image*);
        void drawInfoBox(Image*);
        bool visible(Image*);
        bool selectImage(void);
        void unselectImage(void);
        void relayout(int);
        void drawProgressBar(void);
};
