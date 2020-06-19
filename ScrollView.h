#pragma once

#include "Stage.h"
#include "ImageCache.h"

class ScrollView : public Stage
{
    public:
        ScrollView(ImageCache&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);

    private:
        ImageCache& imageCache;
        sf::RenderWindow& window;

        //int offset;
};
