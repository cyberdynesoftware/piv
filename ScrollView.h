#pragma once

#include "Stage.h"
#include "ImageCache.h"
#include "ImageData.h"

class ScrollView : public Stage
{
    public:
        ScrollView(ImageCache&, sf::RenderWindow&);

        bool instanceOf(const SubType&);
        void handle(sf::Event&);
        void draw(void);
        void selectImage(void);
        void scrollToCurrentImage(void);

    private:
        ImageCache& imageCache;
        sf::RenderWindow& window;
        int numberOfColumns = 5;
};
