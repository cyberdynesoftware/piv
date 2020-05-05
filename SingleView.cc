#include "SingleView.h"

SingleView::SingleView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
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
                    folder.next();
                    break;
                case sf::Keyboard::Backspace:
                    folder.previous();
                    break;
                case sf::Keyboard::R:
                    folder.random();
                    break;
                case sf::Keyboard::A:
                    folder.select();
                    folder.next();
                    break;
                case sf::Keyboard::D:
                    folder.trash();
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            folder.getImage()->zoom(event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    folder.getImage()->fitToScreen();
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
                folder.getImage()->move(sf::Mouse::getPosition() - previousMousePosition);

            previousMousePosition = sf::Mouse::getPosition();
            break;

        default:
            break;
    }
}
