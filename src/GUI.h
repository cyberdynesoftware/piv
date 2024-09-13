#pragma once

#include "Image.h"
#include "TextWithBackground.h"

class GUI : public sf::Drawable
{
    public:
        GUI(sf::RenderWindow& window);

        void update(void);

        void drawProgressBar(float progress, std::string msg);
        void helpMsg(std::string help);

        bool showHelp = false;
        bool showSelectedFolderWarning = false;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RenderWindow& window;
        sf::Clock selectedFolderWarningClock;

        sf::RectangleShape progressBar;
        TextWithBackground progressLabel = TextWithBackground(TextWithBackground::Config::progress, 15);
        TextWithBackground help = TextWithBackground(TextWithBackground::Config::help, 18);
        TextWithBackground warning = TextWithBackground(TextWithBackground::Config::warning, 20);

        int selectedFolderWarningCounter = 0;
};
