#pragma once

#include "SFML/Window.hpp"
#include "ImageManager.h"
#include "MultiImageView.h"

class SceneManager
{
    public:
        SceneManager(Folder& folder, sf::RenderWindow& window);

        void process(const sf::Event& event);

        void update(void);

    private:
        sf::RenderWindow& window;
        ImageManager imageManager;
        MultiImageView multiImageView;

        ImageView& eventReceiver;
};
