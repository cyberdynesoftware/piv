#pragma once

#include "SFML/Window.hpp"
#include "MultiImageView.h"
#include "SingleImageView.h"
#include "GUI.h"
#include "Camera.h"
#include "ImageManager.h"

class SceneManager
{
    public:
        SceneManager(Folder& folder, sf::RenderWindow& window);

        void checkFolder(const Folder& folder);
        void process(const sf::Event& event);
        void update(void);
        void resize(int width, int height);

    private:
        sf::RenderWindow& window;
        GUI gui;
        ImageManager imageManager;
        Camera camera;

        MultiImageView multiImageView;
        SingleImageView singleImageView;
        ImageView* eventReceiver;
        bool singleImageViewActive = false;

        ImageManager::SORT_ORDER sortOrder = ImageManager::SORT_ORDER::NAME;
        sf::Clock clock;

        void activateSingleImageView(void);
        void deactivateSingleImageView(void);
        void toggleSortOrder(void);
        std::string sortOrderToString(ImageManager::SORT_ORDER sortOrder) const;
};
