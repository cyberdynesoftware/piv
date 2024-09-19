#pragma once

#include "SFML/Window.hpp"
#include "MultiImageView.h"
#include "SingleImageView.h"

class SceneManager
{
    public:
        SceneManager(Folder& folder, sf::RenderWindow& window);

        void process(const sf::Event& event);
        void update(void);

    private:
        sf::RenderWindow& window;
        GUI gui;
        ImageManager imageManager;

        MultiImageView multiImageView;
        SingleImageView singleImageView;
        ImageView* eventReceiver;
        bool singleImageViewActive = false;

        sf::Clock clock;

        void activateSingleImageView(void);
        void deactivateSingleImageView(void);
};
