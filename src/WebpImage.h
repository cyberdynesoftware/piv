#pragma once

#include <webp/demux.h>
#include <SFML/Graphics.hpp>
#include "Image.h"
#include "AnimatedImage.h"

class WebpImage : public Image, AnimatedImage
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

        void loadFile(const char*);
};
