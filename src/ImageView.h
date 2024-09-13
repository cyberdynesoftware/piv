#pragma once

#include "SFML/Window.hpp"
#include "TextWithBackground.h"
#include "Image.h"
#include <memory>

class ImageView
{
    public:
        virtual void process(const sf::Event& event) = 0;

        static bool showInfo;

    protected:
        TextWithBackground info = TextWithBackground(TextWithBackground::Config::info, 15);

        void setupInfoBox(const std::unique_ptr<Image>& image);
};
