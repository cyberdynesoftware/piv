#include "SingleView.h"
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <iostream>

SingleView::SingleView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
    arrow.loadFromSystem(sf::Cursor::Arrow);
    cross.loadFromSystem(sf::Cursor::Cross);
    font.loadFromFile("font.ttf");
    text.setFont(font);
    text.setCharacterSize(15);
    text.setPosition(10, 10);
    previousMousePosition = sf::Mouse::getPosition();
}

bool
SingleView::instanceOf(const SubType& subType)
{
    return subType == SubType::SingleView;
}

void
SingleView::initImage()
{
    window.setTitle(*folder.currentItem + " - piv");

    image.init(*folder.currentItem);
    image.fitTo(window.getSize());
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
                    if (++folder.currentItem == folder.cend())
                        folder.currentItem--;
                    initImage();
                    break;
                case sf::Keyboard::Backspace:
                    if (folder.currentItem != folder.cbegin())
                        folder.currentItem--;
                    initImage();
                    break;
                case sf::Keyboard::R:
                    //imageCache.random();
                    //fitToScreen((**imageCache.currentImage).getSprite());
                    break;
                case sf::Keyboard::A:
                    //folder.select();
                    break;
                case sf::Keyboard::D:
                    //folder.trash();
                    break;
                case sf::Keyboard::O:
                    image.sprite.setScale(1.0, 1.0);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            zoom(image.sprite, event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    image.fitTo(window.getSize());
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
            {
                sf::Vector2i delta(sf::Mouse::getPosition() - previousMousePosition);
                image.sprite.move(delta.x, delta.y);
            }

            previousMousePosition = sf::Mouse::getPosition();
            break;

        default:
            break;
    }
}

void
SingleView::draw()
{
    if (image.valid)
    {
        image.update();
        window.draw(image.sprite);
    }
    else
    {
        text.setString(image.errormsg);
        window.draw(text);
    }
}

void
SingleView::fullscreenToggle()
{
    window.setTitle(*folder.currentItem + " - piv");

    image.fitTo(window.getSize());
}

void
SingleView::zoom(sf::Sprite& sprite, float delta)
{
    if (delta < 0)
    {
        sprite.scale(0.95f, 0.95f);
    }
    else if (delta > 0)
    {
        sprite.setOrigin(mousePositionInSprite(sprite) / sprite.getScale().x);
        sprite.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        sprite.scale(1.05f, 1.05f);
    }
}

sf::Vector2f
SingleView::mousePositionInSprite(sf::Sprite& sprite)
{
    return sf::Vector2f(sf::Mouse::getPosition().x - sprite.getGlobalBounds().left,
            sf::Mouse::getPosition().y - sprite.getGlobalBounds().top);
}
