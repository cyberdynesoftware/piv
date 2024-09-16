#include "ViewScrollingManager.h"

void
ViewScrollingManager::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            setPosition(it.getCenter().y - event.mouseWheelScroll.delta * it.getSize().y / 100);
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
                    setPosition(it.getSize().y / 2);
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
ViewScrollingManager::update()
{
    switch (scrollState)
    {
        case UP:
            scrollSpeed = it.getSize().y / -50;
            break;
        case DOWN:
            scrollSpeed = it.getSize().y / 50;
            break;
        case UP_FAST:
            scrollSpeed = it.getSize().y / -25;
            break;
        case DOWN_FAST:
            scrollSpeed = it.getSize().y / 25;
            break;
        case AUTO_SCROLL:
            scrollSpeed = it.getSize().y / 800;
            if (scrollSpeed == 0) scrollSpeed = 1;
            break;
        default:
            break;
    }

    if (scrollSpeed != 0)
    {
        setPosition(it.getCenter().y + scrollSpeed);
        scrollSpeed = scrollSpeed / 2;
        return true;
    }

    return false;
}

void
ViewScrollingManager::setPosition(int centerY)
{
    it.setCenter(it.getCenter().x, centerY);

    if (getTop() < 0 || bottom < it.getSize().y)
        it.setCenter(it.getCenter().x, it.getSize().y / 2);
    else if (/*(folderIter == folder.cend() || showSelection) &&*/ it.getCenter().y + it.getSize().y / 2 > bottom)
        it.setCenter(it.getCenter().x, bottom - it.getSize().y / 2);
}

float
ViewScrollingManager::getTop()
{
    return it.getCenter().y - it.getSize().y / 2.f;
}

float
ViewScrollingManager::getBottom()
{
    return it.getCenter().y + it.getSize().y / 2.f;
}

bool
ViewScrollingManager::isVisible(const std::unique_ptr<Image>& image)
{
    const auto& bounds = image->sprite.getGlobalBounds();
    return (bounds.top + bounds.height > getTop() && bounds.top < getBottom());
}
