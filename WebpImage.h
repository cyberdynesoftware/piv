#pragma once

#include <webp/demux.h>
#include <SFML/Graphics.hpp>
#include "AnimatedImage.h"

class WebpImage : public AnimatedImage
{
    public:
        WebpImage(const char*);
        ~WebpImage();

        bool isWebp(void);
        void prepare(sf::Sprite&);
        void update(sf::Time, sf::Sprite&);

    private:
        WebPData data;
        WebPAnimDecoder* decoder = nullptr;
        int previousTimestamp = 0;
        sf::Texture texture;

        void loadFile(const char*);
};
