#pragma once

#include "ImageView.h"
#include "Image.h"
#include "Folder.h"
#include <SFML/Graphics.hpp>

class SingleImageView : public ImageView
{
    public:
        SingleImageView(Folder&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        void initImage(void);
        void resizeEvent(void);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        Image* image = nullptr;
        sf::Cursor arrow;
        sf::Cursor cross;
        sf::Vector2i previousMousePosition;
        bool showInfo = false;
        bool fullscreen = true;

        void zoom(sf::Sprite&, float delta);
        sf::Vector2f mousePositionInSprite(const sf::Sprite&);
        void original(sf::Sprite&, bool);
        void toggleFit(bool);
        void next(void);
        void previous(void);
};
