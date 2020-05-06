#include "SingleView.h"

SingleView::SingleView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    arrow.loadFromSystem(sf::Cursor::Arrow);
    cross.loadFromSystem(sf::Cursor::Cross);
    previousMousePosition = sf::Mouse::getPosition();
}

void
SingleView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Space:
                    imageCache.next();
                    break;
                case sf::Keyboard::Backspace:
                    imageCache.previous();
                    break;
                case sf::Keyboard::R:
                    imageCache.random();
                    break;
                case sf::Keyboard::A:
                    //folder.select();
                    imageCache.next();
                    break;
                case sf::Keyboard::D:
                    //folder.trash();
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            imageCache.getCurrent()->zoom(event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    imageCache.getCurrent()->fitToScreen();
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(arrow);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseMoved:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                imageCache.getCurrent()->move(sf::Mouse::getPosition() - previousMousePosition);

            previousMousePosition = sf::Mouse::getPosition();
            break;

        default:
            break;
    }
}

void
SingleView::draw()
{
    window.draw(imageCache.getCurrent()->getSprite());
}
