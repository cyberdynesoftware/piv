#pragma once

#include "Image.h"
#include "ImageView.h"
#include "ImageManager.h"
#include "Camera.h"
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>

class MultiImageView : public ImageView
{
    public:
        MultiImageView(sf::RenderWindow&, ImageManager&, Camera&);

        void process(const sf::Event&);
        void draw(void);
        void resize(int width, int height);
        std::deque<std::unique_ptr<Image>>::iterator findImageUnderMouse(void) const;
        std::pair<int, int> calcProgress(void);

    private:
        sf::RenderWindow& window;
        ImageManager& imageManager;
        Camera& camera;

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

        ImageManager::SORT_ORDER sortOrder = ImageManager::SORT_ORDER::NAME;

        void layout(std::unique_ptr<Image>& image);
        void relayoutImages(int);
        void markSelectedImage(const std::unique_ptr<Image>& image);
        void selectImage(void);
        void toggleShowSelection(void);
        void clearSelection(void);
        void moveSelectedImages(void);
        void toggleSortOrder(void);
        void reset(void);
};
