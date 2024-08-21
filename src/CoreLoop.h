#pragma once

#include "MultiImageView.h"
#include "Folder.h"
#include "InputEvent.h"
#include "WindowManager.h"

class CoreLoop : WindowManager
{
    private:
        sf::RenderWindow window;
        sf::Image icon;
        bool fullscreen = false;

        Folder folder;
        MultiImageView imageView;
        InputEvent inputEvent;

    public:
        CoreLoop(char* path);

        virtual void closeWindow(void);
        virtual void toggleFullscreen(void);

        void runCoreLoop(void);
};
