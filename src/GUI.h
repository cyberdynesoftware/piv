#pragma once

#include "Image.h"
#include "TextWithBackground.h"

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
        bool showSelectedFolderWarning = false;

    private:
        sf::RectangleShape progressBar;
        TextWithBackground progressLabel = TextWithBackground(TextWithBackground::Config::progress, 15);
        TextWithBackground info = TextWithBackground(TextWithBackground::Config::info, 15);
        TextWithBackground help = TextWithBackground(TextWithBackground::Config::help, 18);
        TextWithBackground warning = TextWithBackground(TextWithBackground::Config::warning, 20);

        sf::Color pumpkin = sf::Color(245, 118, 26);
        sf::CircleShape selectedIconCircle;

        int selectedFolderWarningCounter = 0;
};
