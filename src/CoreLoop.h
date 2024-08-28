#pragma once

#include "MultiImageView.h"
#include "Folder.h"
#include "InputEvent.h"
#include "WindowManager.h"

class CoreLoop : WindowManager
{
    public:
        CoreLoop(char* path);

        virtual void closeWindow(void);
        virtual void toggleFullscreen(void);

        void runCoreLoop(void);

    private:
        void setupFullscreen(void);
        void setupWindow(void);
        sf::Vector2i windowPos(void);

        sf::RenderWindow window;
        sf::Image icon;
        bool fullscreen = false;
        const sf::VideoMode windowMode = sf::VideoMode(1280, 720);

        Folder folder;
        MultiImageView imageView;
        InputEvent inputEvent;
};
