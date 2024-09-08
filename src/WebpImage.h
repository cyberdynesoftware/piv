#pragma once

#include <webp/demux.h>
#include <SFML/Graphics.hpp>
#include "Image.h"
#include "AnimatedImage.h"

class WebpImage : public AnimatedImage
{
    public:
        ~WebpImage();

        virtual void init(const std::string& p);
        bool isWebp(void);
        virtual void prepare(void);

    private:
        WebPData data;

        void loadFile(const char*);
};
