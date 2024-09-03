#pragma once

#include "Image.h"
#include "ImageView.h"
#include "GUI.h"
#include "ImageManager.h"
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>

class MultiImageView : public ImageView
{
    public:
        MultiImageView(sf::RenderWindow&, ImageManager& imageManager, GUI&);

        void process(const sf::Event&);
        void draw(void);
        void resize(void);
        Image* findImageUnderMouse(void);

    private:
        GUI& gui;
        ImageManager& imageManager;
        sf::View view;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::vector<int> columnOffsets;
        int columnIndex = 0;
        int scrollSpeed = 0;
        int bottom = 0;
        float viewPosition;
        float viewHeight;
        int targetImageWidth;
        Image* lastVisibleImage;
        enum SCROLL_STATES { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATES scrollState = NONE;
        bool showSelection = false;
        float lastViewPosition;
        sf::RectangleShape highlightBackground;

        void scrollView();
        int minColumnIndex(void);
        void layout(Image*);
        bool isVisible(Image*);
        void relayoutImages(int);
        void setViewPosition(int);
        void highlight(Image*);
        float calcProgress(void);
};
