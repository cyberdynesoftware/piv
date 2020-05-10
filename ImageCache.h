#pragma once

#include "Folder.h"
#include "AnimatedGIF.h"

class ImageCache
{
    public:
        ImageCache(const char* path);

        sf::Sprite& getSprite(void);
        void next(void);
        void previous(void);
        void random(void);
        void update(void);

    private:
        Folder folder;
        sf::Texture texture;
        sf::Sprite sprite;
        bool currentIsGIF;
        std::unique_ptr<AnimatedGIF> gif;

        void load(void);
};
