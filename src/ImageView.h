#pragma once

#include "SFML/Window.hpp"
#include "Notification.h"
#include "Image.h"
#include <memory>

class ImageView
{
    public:
        virtual void process(const sf::Event& event) = 0;

        static bool showInfo;

        virtual std::string helpMsg(void) = 0;

    protected:
        Notification info;

        void setupInfoBox(const std::unique_ptr<Image>& image);
};
