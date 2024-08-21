#pragma once

#include "WindowManager.h"
#include "MultiImageView.h"

class InputEvent
{
    private:
        WindowManager& windowManager;
        MultiImageView& imageView;

    public:
        InputEvent(WindowManager& windowManager, MultiImageView& imageView);

        void process(const sf::Event& event);
};
