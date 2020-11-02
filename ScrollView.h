#pragma once

#include "Stage.h"
#include "Image.h"
#include "Folder.h"
#include <deque>

class ScrollView : public Stage
{
    public:
        ScrollView(Folder&, sf::RenderWindow&);

        bool instanceOf(const SubType&);
        void handle(sf::Event&);
        void draw(void);
        void resizeEvent(void);
        void selectImage(void);
        void scrollToCurrentImage(void);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        int heightOffset = 0;
        std::deque<Image*> images;
        Folder::FolderIter firstItem;
        Folder::FolderIter lastItem;

        void initImages(void);
        void scrollDown(int);
        void scrollUp(int);
        int imageSize(void);
};
