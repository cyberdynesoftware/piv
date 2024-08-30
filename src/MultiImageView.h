#pragma once

#include "Image.h"
#include "ImageView.h"
#include "Folder.h"
#include "GUI.h"
#include "ImageManager.h"
#include <deque>
#include <vector>

class MultiImageView : public ImageView
{
    public:
        MultiImageView(Folder&, sf::RenderWindow&, ImageManager& imageManager);

        void process(const sf::Event&);
        void draw(void);
        void resize(void);

    private:
        GUI gui;
        Folder& folder;
        ImageManager& imageManager;
        Folder::iter folderIter;
        sf::View view;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::deque<Image*> images;
        std::deque<Image*>::iterator imageIter;
        std::vector<int> columnOffsets;
        int columnIndex = 0;
        int scrollSpeed = 0;
        int bottom = 0;
        float viewPosition;
        float viewHeight;
        int targetImageWidth;
        Image* elevatedImage = NULL;
        Image* lastVisibleImage;
        enum SCROLL_STATES { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATES scrollState = NONE;
        bool showSelection = false;
        float lastViewPosition;
        sf::RectangleShape highlightBackground;
        sf::Vector2i previousMousePosition;

        //void loadImageRow(void);
        void scrollView();
        int minColumnIndex(void);
        void layout(Image*);
        bool isVisible(Image*);
        void pickImage(void);
        void unpickImage(void);
        void relayoutImages(int);
        void setViewPosition(int);
        Image* findImageUnderMouse(void);
        void selectImage(void);
        void highlight(Image*);
        void nextImage(void);
        void previousImage(void);
        void zoom(float);
        float calcProgress(void);
        std::string generateHelpText(void);
};
