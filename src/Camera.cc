#include "Camera.h"

Camera::Camera()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){ 0.f, 0.f };
    worldId = b2CreateWorld(&worldDef);

    createCameraBody(toB2BodyPos(view.getCenter().y));

    topGuardBodyId = createStaticBody(toB2BodyPos(view.getCenter().y) - springLength);
    topGuardSpringJointId = createSpringJoint(topGuardBodyId, cameraBodyId);

    bottomGuardBodyId = createStaticBody(0.f);
    bottomGuardSpringJointId = createSpringJoint(bottomGuardBodyId, cameraBodyId);
    b2Body_Disable(bottomGuardBodyId);
}

void
Camera::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            if (bottom > view.getSize().y)
            {
                auto newtons = event.mouseWheelScroll.delta < 0 ? 5.f : -5.f;
                b2Body_ApplyLinearImpulseToCenter(cameraBodyId, (b2Vec2){ 0.f, newtons }, true);
            }
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::XButton1:
                    scrollState = DOWN;
                    break;
                case sf::Mouse::Button::XButton2:
                    scrollState = UP;
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::XButton1:
                case sf::Mouse::Button::XButton2:
                    scrollState = NONE;
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                    scrollState = UP;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    scrollState = DOWN;
                    break;
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                    scrollState = UP_FAST;
                    break;
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollState = DOWN_FAST;
                    break;
                case sf::Keyboard::Space:
                    scrollState = (scrollState == NONE) ? AUTO_SCROLL : NONE;
                    break;
                case sf::Keyboard::Home:
                case sf::Keyboard::G:
                    b2Body_Enable(topGuardBodyId);
                    scrollState = TOP;
                    break;
                default:
                    break;
            }
            break;
            
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollState = NONE;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void
Camera::applyForce()
{
    auto newtons = 0.f;
    switch (scrollState)
    {
        case UP:
            newtons = -16.f;
            break;
        case DOWN:
            newtons = 16.f;
            break;
        case UP_FAST:
            newtons = -40.f;
            break;
        case DOWN_FAST:
            newtons = 40.f;
            break;
        case AUTO_SCROLL:
            newtons = 2.f;
            break;
        default:
            break;
    }
    b2Body_ApplyForceToCenter(cameraBodyId, (b2Vec2){ 0.f, newtons }, true);
}

bool
Camera::update(const sf::Time& time)
{
    enforceTopGuardConstraint();

    if (bottom > view.getSize().y)
    {
        enforceBottomGuardConstraint();
    }

    if (b2Body_IsValid(adjustBodyId) && (!b2Body_IsAwake(cameraBodyId) || scrollState != ADJUST))
    {
        b2DestroyBody(adjustBodyId);
    }

    if (scrollState != NONE && bottom > view.getSize().y)
    {
        applyForce();
    }

    auto timeStep = (time - lastUpdate).asSeconds();
    lastUpdate = time;

    b2World_Step(worldId, timeStep, 1);

    auto events = b2World_GetBodyEvents(worldId);
    for (int i = 0; i < events.moveCount; i++)
    {
        const auto* event = events.moveEvents + i;
        if (B2_ID_EQUALS(event->bodyId, cameraBodyId))
        {
            view.setCenter(view.getCenter().x, toViewCenter(event->transform.p.y));
            return true;
        }
    }

    return false;
}

float
Camera::toViewCenter(float y)
{
    return view.getSize().y * y / 10.f;
}

float
Camera::toB2BodyPos(float y)
{
    return y * 10.f / view.getSize().y;
}

void
Camera::enforceTopGuardConstraint()
{
    if (getTop() < 0.f)
    {
        if (!b2Body_IsEnabled(topGuardBodyId))
        {
            b2Body_Enable(topGuardBodyId);
        }
    }
    else
    {
        if (b2Body_IsEnabled(topGuardBodyId) && scrollState != TOP)
        {
            b2Body_Disable(topGuardBodyId);
        }
    }
}

void
Camera::enforceBottomGuardConstraint()
{
    if (getBottom() > bottom)
    {
        if (!b2Body_IsEnabled(bottomGuardBodyId))
        {
            b2Body_SetTransform(bottomGuardBodyId,
                    (b2Vec2){ 0, toB2BodyPos(bottom - view.getSize().y / 2) - springLength },
                    b2Body_GetRotation(bottomGuardBodyId));
            b2Body_Enable(bottomGuardBodyId);
        }
    }
    else
    {
        if (b2Body_IsEnabled(bottomGuardBodyId))
        {
            b2Body_Disable(bottomGuardBodyId);
        }
    }
}

void
Camera::adjustPosition(float top)
{
    auto y = toB2BodyPos(top);
    y += getTop() > top ? springLength : -springLength;
    adjustBodyId = createStaticBody(y);
    adjustSpringJointId = createSpringJoint(adjustBodyId, cameraBodyId);
    b2Body_SetAwake(cameraBodyId, true);
    scrollState = ADJUST;
}

void
Camera::teleport(float center)
{
    auto y = toB2BodyPos((center > view.getSize().y / 2.f) ? center : view.getSize().y / 2.f);
    b2Body_SetTransform(cameraBodyId, (b2Vec2){ 0, y }, b2Body_GetRotation(cameraBodyId));
    b2Body_SetAwake(cameraBodyId, true);
}

float
Camera::getTop()
{
    return view.getCenter().y - view.getSize().y / 2.f;
}

float
Camera::getBottom()
{
    return view.getCenter().y + view.getSize().y / 2.f;
}

bool
Camera::isVisible(const std::unique_ptr<Image>& image)
{
    const auto& bounds = image->sprite.getGlobalBounds();
    return (bounds.top + bounds.height > getTop() && bounds.top < getBottom());
}

void
Camera::createCameraBody(float y)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){ 0.f, y };
    bodyDef.linearDamping = 5.f;
    bodyDef.fixedRotation = true;

    cameraBodyId = b2CreateBody(worldId, &bodyDef);

    b2MassData massData;
    massData.mass = 1.f;
    massData.center = (b2Vec2){ 0.f, 0.f };
    massData.rotationalInertia = 0.f;
    b2Body_SetMassData(cameraBodyId, massData);
}

b2BodyId
Camera::createStaticBody(float y)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = (b2Vec2){ 0.f, y };
    return b2CreateBody(worldId, &bodyDef);
}

b2JointId
Camera::createSpringJoint(b2BodyId a, b2BodyId b)
{
    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.bodyIdA = a;
    jointDef.bodyIdB = b;
    jointDef.collideConnected = false;
    jointDef.length = springLength;
    jointDef.enableSpring = true;
    jointDef.hertz = 1.f;
    jointDef.dampingRatio = 1.f;
    return b2CreateDistanceJoint(worldId, &jointDef);
}
