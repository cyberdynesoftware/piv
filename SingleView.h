#pragma once

#include "Stage.h"
#include "ImageCache.h"

class SingleView : public Stage
{
    public:
        SingleView(ImageCache&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);

    private:
        ImageCache& imageCache;
        sf::RenderWindow& window;
        sf::Cursor arrow;
        sf::Cursor cross;
        sf::Vector2i previousMousePosition;
};
