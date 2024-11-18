#pragma once

#include "Image.h"
#include "TextWithBackground.h"
#include "Notification.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class GUI : public sf::Drawable
{
    public:
        GUI(sf::RenderWindow& window);

        void update(const sf::Time& time);

        void drawProgressBar(int index, int max);
        void showHelpMsg(std::string help);

        bool showHelp = false;

        std::unique_ptr<Notification> notification = nullptr;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RenderWindow& window;

        sf::RectangleShape progressBar;
        TextWithBackground progressLabel = TextWithBackground(TextWithBackground::Config::progress, 15);
        TextWithBackground help = TextWithBackground(TextWithBackground::Config::help, 18);
};
