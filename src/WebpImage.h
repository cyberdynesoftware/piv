#pragma once

#include <webp/demux.h>
#include <SFML/Graphics.hpp>
#include "Image.h"

class WebpImage : public Image
{
    public:
        ~WebpImage();

        bool isWebp(void);
        virtual void init(const std::string& p);
        virtual void prepare(void);
        virtual void load(const sf::Time& time);
        virtual void update(const sf::Time& time);

    private:
        WebPData data;
        WebPAnimDecoder* decoder = nullptr;
        bool animate = false;
        sf::Time lastFrameUpdate;
        sf::Time delay;
        int previousTimestamp = 0;

        void loadFile(const char*);
};
