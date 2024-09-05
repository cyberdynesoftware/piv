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
        sf::Clock clock;

        bool singleImageViewActive = false;
        ImageView* eventReceiver;

        void activateSingleImageView(void);
        void deactivateSingleImageView(void);
};
