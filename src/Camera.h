#pragma once

#include "GUI.h"
#include "Image.h"
#include "SFML/Graphics.hpp"
#include "box2d/box2d.h"

class Camera
{
    public:
        ViewScrollingManager();

        void process(const sf::Event&);
        bool update(void);
        bool isVisible(const std::unique_ptr<Image>& image);
        void setPosition(int);
        float getTop(void);
        float getBottom(void);

        sf::View view;

        float bottom = 0.f;

    private:
        int scrollSpeed = 0;

        enum SCROLL_STATES { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATES scrollState = NONE;

        b2WorldId worldId;
};
