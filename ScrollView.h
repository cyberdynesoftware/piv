#pragma once

#include "Stage.h"
#include "ImageCache.h"

class ScrollView : public Stage
{
    public:
        ScrollView(ImageCache&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        void selectImage(void);
        void scrollToCurrentImage(void);

    private:
        ImageCache& imageCache;
        sf::RenderWindow& window;

        std::vector<std::vector<sf::Sprite*>> spreadImagesToRows(void);
        int layoutRow(std::vector<sf::Sprite*>&, int);
};
