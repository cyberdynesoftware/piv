#pragma once

#include "SFML/Window.hpp"
#include "SceneManager.h"
#include "Folder.h"

class CoreLoop
{
    public:
        CoreLoop(char* path);

        void runCoreLoop(void);

    private:
        void toggleFullscreen(void);
        sf::Vector2i desktopCenterPosition(void);
        void process(const sf::Event& event);

        sf::RenderWindow window;
        sf::Image icon;
        bool fullscreen = false;
        const sf::VideoMode windowMode = sf::VideoMode(1280, 720);

        Folder folder;
        SceneManager sceneManager;
};
