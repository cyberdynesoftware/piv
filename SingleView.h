#pragma once

#include "Stage.h"
#include "ImageCache.h"

class SingleView : public Stage
{
    public:
        SingleView(Folder&, sf::RenderWindow&);

        bool instanceOf(const SubType&);
        void handle(sf::Event&);
        void draw(void);
        void init(void);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Cursor arrow;
        sf::Cursor cross;
        sf::Vector2i previousMousePosition;

        bool loadImage(const std::string&);
        void fitToScreen(sf::Sprite&);
        void zoom(sf::Sprite&, float delta);
        sf::Vector2f mousePositionInSprite(sf::Sprite&);
};
