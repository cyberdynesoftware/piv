#pragma once

#include "Image.h"

class GUI
{
    public:
        GUI();

        void drawInfoBox(sf::RenderWindow& window, Image* image);
        void drawProgressBar(sf::RenderWindow& window, float progress, std::string msg);
        void drawSelectedIcon(sf::RenderWindow& window, Image*);
        void drawHelpText(sf::RenderWindow& window, std::string help);
        void selectedFolderWarning(sf::RenderWindow& window);

        bool showInfo = false;
        bool showHelp = false;

    private:
        sf::Font font;
        int progressBarWidth = 15;
        sf::RectangleShape progressBar;
        int selectedFolderWarningCounter = 0;
};
