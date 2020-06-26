#pragma once

#include "Stage.h"
#include "ImageCache.h"

class ScrollView : public Stage
{
    public:
        ScrollView(ImageCache&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        ImageCache::ImageIter getSelected(void);

    private:
        ImageCache& imageCache;
        sf::RenderWindow& window;

        void assignToRow(void);
        int layoutRow(std::vector<sf::Sprite*>&, int);
};
