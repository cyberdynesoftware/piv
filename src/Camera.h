#pragma once

#include "GUI.h"
#include "Image.h"
#include "SFML/Graphics.hpp"
#include "box2d/box2d.h"

class Camera
{
    public:
        Camera();

        void process(const sf::Event&);
        bool update(const sf::Time& time);
        bool isVisible(const std::unique_ptr<Image>& image);
        void setPosition(int);
        float getTop(void);
        float getBottom(void);

        sf::View view;

        float bottom = 0.f;

    private:
        enum SCROLL_STATE { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL };
        SCROLL_STATE scrollState = NONE;

        b2WorldId worldId;
        b2BodyId cameraBodyId;
        sf::Time lastUpdate;

        void applyForce(void);
        void createCameraBody(void);
};
