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
        void adjustPosition(float top);
        void teleport(float center);
        float getTop(void);
        float getBottom(void);

        sf::View view;

        float bottom = 0.f;

    private:
        enum SCROLL_STATE { NONE, UP, UP_FAST, DOWN, DOWN_FAST, AUTO_SCROLL, TOP, ADJUST };
        SCROLL_STATE scrollState = NONE;

        sf::Time lastUpdate;
        b2WorldId worldId;
        b2BodyId cameraBodyId;
        b2BodyId topGuardBodyId;
        b2JointId topGuardSpringJointId;
        b2BodyId bottomGuardBodyId;
        b2JointId bottomGuardSpringJointId;
        b2BodyId adjustBodyId;
        b2JointId adjustSpringJointId;
        const float springLength = 10.f;

        float toViewCenter(float y);
        float toB2BodyPos(float y);
        void createCameraBody(float y);
        void applyForce(void);
        void enforceTopGuardConstraint(void);
        void enforceBottomGuardConstraint(void);
        b2BodyId createStaticBody(float y);
        b2JointId createSpringJoint(b2BodyId a, b2BodyId b);
};
