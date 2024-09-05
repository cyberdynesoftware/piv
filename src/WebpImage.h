#pragma once

#include <webp/demux.h>
#include <SFML/Graphics.hpp>
#include "Image.h"

class WebpImage : public Image
{
    public:
        ~WebpImage();

        bool isWebp(void);
        virtual void init(const char*);
        virtual void prepare(void);
        virtual void update(const sf::Time& time);

    private:
        WebPData data;
        WebPAnimDecoder* decoder = nullptr;
        bool animate = false;
        sf::Time lastFrameUpdate = sf::milliseconds(0);
        int lastTimestamp = 0;

        void loadFile(const char*);
};
