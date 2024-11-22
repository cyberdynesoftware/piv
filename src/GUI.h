#pragma once

#include "Image.h"
#include "Notification.h"
#include "Dialog.h"
#include "ProgressBarLabel.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class GUI : public sf::Drawable
{
    public:
        GUI(sf::RenderWindow& window);

        void update(const sf::Time& time);

        void drawProgressBar(std::pair<int, int> progress);
        void showHelpMsg(std::string help);
        void dismissHelpMsg(void);

        std::unique_ptr<Notification> notification = nullptr;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RenderWindow& window;

        std::unique_ptr<Dialog> dialog = nullptr;
        sf::RectangleShape progressBar;
        ProgressBarLabel progressBarLabel;
};
