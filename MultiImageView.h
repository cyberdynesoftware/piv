#pragma once

#include "ImageView.h"
#include "Image.h"
#include "Folder.h"
#include <deque>

class MultiImageView : public ImageView
{
    public:
        MultiImageView(Folder&, sf::RenderWindow&);

        void handle(sf::Event&);
        void draw(void);
        void resizeEvent(void);
        bool selectImage(void);
        void scrollToCurrentImage(void);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        int numberOfColumns = 4;
        int heightOffset = 0;
        std::deque<Image*> images;
        Folder::FolderIter firstItem;
        Folder::FolderIter lastItem;
        bool showInfo = false;

        void initImages(void);
        void scrollDown(void);
        void scrollUp(void);
        void scrollTo(const Folder::FolderIter&);
        int imageSize(void);
};
