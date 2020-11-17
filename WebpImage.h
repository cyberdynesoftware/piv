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
        void prepare(sf::Texture&);
        void update(sf::Texture&);

    private:
        WebPData data;
        WebPAnimDecoder* decoder = nullptr;
        int previousTimestamp;

        void loadFile(const char*);
};
