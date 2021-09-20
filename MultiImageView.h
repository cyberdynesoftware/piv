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
        int progressBarWidth = 12;
        sf::RectangleShape progressBar;
        Image* lastVisibleImage;
        enum SCROLL_STATES { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATES scrollState = NONE;
        bool showSelection = false;
        float lastViewPosition;
        bool showHelp = false;
        int selectedFolderWarningCounter = 0;

        void loadImageRow(void);
        void scrollView();
        int minColumnIndex(void);
        void layout(Image*);
        void drawInfoBox(Image*);
        bool isVisible(Image*);
        void pickImage(void);
        void unpickImage(void);
        void relayoutImages(int);
        void drawProgressBar(void);
        void setViewPosition(int);
        void drawSelectedIcon(Image*);
        void drawHelpText(void);
        void selectedFolderWarning(void);
};
