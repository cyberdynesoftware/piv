#pragma once

#include "Image.h"
#include "ImageView.h"
#include "GUI.h"
#include "ImageManager.h"
#include "ViewScrollingManager.h"
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
        ViewScrollingManager view;
        GUI& gui;

        int numberOfColumns = 4;
        std::vector<int> columnOffsets;
        int columnIndex = 0;
        int targetImageWidth;
        std::unique_ptr<Image>* lastVisibleImage;
        bool showSelection = false;
        float lastViewPosition;

        sf::RectangleShape selectedImageForeground;
        sf::Color pumpkin = sf::Color(245, 118, 26);
        sf::CircleShape selectedIconCircle;

        int minColumnIndex(void);
        void layout(std::unique_ptr<Image>& image);
        void relayoutImages(int);
        void markSelectedImage(const std::unique_ptr<Image>& image);
        void calcProgress(void);
};
