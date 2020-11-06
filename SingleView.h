#pragma once

#include "Stage.h"
#include "Image.h"
#include "Folder.h"
#include <SFML/Graphics.hpp>

class SingleView : public Stage
{
    public:
        SingleView(Folder&, sf::RenderWindow&);

        bool instanceOf(const SubType&);
        void handle(sf::Event&);
        void draw(void);
        void initImage(void);
        void resizeEvent(void);

        static std::string filename(const std::string&);

    private:
        Folder& folder;
        Image* image;
        sf::RenderWindow& window;
        sf::Text text;
        sf::Font font;
        sf::Cursor arrow;
        sf::Cursor cross;
        sf::Vector2i previousMousePosition;

        void zoom(sf::Sprite&, float delta);
        sf::Vector2f mousePositionInSprite(sf::Sprite&);
        void original(sf::Sprite&);
};
