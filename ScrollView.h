#pragma once

#include "Stage.h"
#include "Image.h"
#include "Folder.h"

class ScrollView : public Stage
{
    public:
        ScrollView(Folder&, sf::RenderWindow&);

        bool instanceOf(const SubType&);
        void handle(sf::Event&);
        void draw(void);
        void resizeEvent(sf::Event::SizeEvent&);
        void selectImage(void);
        void scrollToCurrentImage(void);
        void scrollToPosition(float);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        std::map<std::string, Image*> imageCache;

        Image* getImage(const std::string&);
        int imageSize(void);
};
