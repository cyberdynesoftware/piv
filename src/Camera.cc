#include "Camera.h"

void
Camera::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            setPosition(view.getCenter().y - event.mouseWheelScroll.delta * view.getSize().y / 100);
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
                    scrollSpeed = 0;
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

bool
Camera::update()
{
    switch (scrollState)
    {
        case UP:
            scrollSpeed = view.getSize().y / -50;
            break;
        case DOWN:
            scrollSpeed = view.getSize().y / 50;
            break;
        case UP_FAST:
            scrollSpeed = view.getSize().y / -25;
            break;
        case DOWN_FAST:
            scrollSpeed = view.getSize().y / 25;
            break;
        case AUTO_SCROLL:
            scrollSpeed = view.getSize().y / 800;
            if (scrollSpeed == 0) scrollSpeed = 1;
            break;
        default:
            break;
    }

    if (scrollSpeed != 0)
    {
        setPosition(view.getCenter().y + scrollSpeed);
        scrollSpeed = scrollSpeed / 2;
        return true;
    }

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
