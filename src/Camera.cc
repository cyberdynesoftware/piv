#include "Camera.h"

Camera::Camera()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){ 0.f, 0.f };
    worldId = b2CreateWorld(&worldDef);
    createCameraBody();
}

void
Camera::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            {
                auto newtons = event.mouseWheelScroll.delta < 0 ? 1.f : -1.f;
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
                    setPosition(view.getSize().y / 2);
                    //scrollSpeed = 0;
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
            newtons = -1.f;
            break;
        case DOWN:
            newtons = 1.f;
            break;
        case UP_FAST:
            newtons = -4.f;
            break;
        case DOWN_FAST:
            newtons = 4.f;
            break;
        case AUTO_SCROLL:
            newtons = 0.5f;
            break;
        default:
            break;
    }
    b2Body_ApplyForceToCenter(cameraBodyId, (b2Vec2){ 0.f, newtons }, true);
}

bool
Camera::update(const sf::Time& time)
{
    if (scrollState != NONE)
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
            float y = view.getSize().y * event->transform.p.y / 10.f + view.getSize().y / 2.f;
            view.setCenter(view.getCenter().x, y);
            return true;
        }
    }

    /*
    if (scrollSpeed != 0)
    {
        setPosition(view.getCenter().y + scrollSpeed);
        scrollSpeed = scrollSpeed / 2;
        return true;
    }
    */

    return false;
}

void
Camera::setPosition(int centerY)
{
    view.setCenter(view.getCenter().x, centerY);

    if (getTop() < 0 || bottom < view.getSize().y)
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    else if (/*(folderIter == folder.cend() || showSelection) &&*/ view.getCenter().y + view.getSize().y / 2 > bottom)
        view.setCenter(view.getCenter().x, bottom - view.getSize().y / 2);
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
Camera::createCameraBody()
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){ 0.f, 0.f };
    bodyDef.linearDamping = 5.f;
    bodyDef.fixedRotation = true;

    cameraBodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon box = b2MakeBox(0.1f, 0.1f);

    b2ShapeDef shape = b2DefaultShapeDef();

    b2CreatePolygonShape(cameraBodyId, &shape, &box);
}
