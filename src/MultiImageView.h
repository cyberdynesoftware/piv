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
        std::unique_ptr<Image>& findImageUnderMouse(void) const;

    private:
        sf::RenderWindow& window;
        ImageManager& imageManager;
        GUI& gui;

        sf::View view;
        int numberOfColumns = 4;
        std::vector<int> columnOffsets;
        int columnIndex = 0;
        int scrollSpeed = 0;
        int bottom = 0;
        float viewPosition;
        float viewHeight;
        int targetImageWidth;
        std::unique_ptr<Image>* lastVisibleImage;
        enum SCROLL_STATES { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATES scrollState = NONE;
        bool showSelection = false;
        float lastViewPosition;

        sf::RectangleShape selectedImageForeground;
        sf::Color pumpkin = sf::Color(245, 118, 26);
        sf::CircleShape selectedIconCircle;

        void scrollView();
        int minColumnIndex(void);
        void layout(std::unique_ptr<Image>& image);
        bool isVisible(const std::unique_ptr<Image>& image);
        void relayoutImages(int);
        void setViewPosition(int);
        void markSelectedImage(const std::unique_ptr<Image>& image);
        void calcProgress(void);
};
