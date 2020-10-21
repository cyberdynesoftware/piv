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
    image = NULL;
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

    if (image != NULL) delete image;
    image = new Image(*folder.currentItem, false);
    image->fitTo(window.getSize());
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
                        folder.currentItem = folder.cbegin();
                    initImage();
                    break;
                case sf::Keyboard::Backspace:
                    if (folder.currentItem != folder.cbegin())
                        folder.currentItem--;
                    else
                        folder.currentItem = --folder.cend();
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
                    original(image->sprite);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            zoom(image->sprite, event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    image->fitTo(window.getSize());
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
                image->sprite.move(delta.x, delta.y);
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
    if (image->valid)
    {
        image->update();
        image->fitTo(window.getSize());
        window.draw(image->sprite);
    }
    else
    {
        text.setString(image->errormsg);
        window.draw(text);
    }
}

void
SingleView::fullscreenToggle()
{
    window.setTitle(*folder.currentItem + " - piv");

    image->fitTo(window.getSize());
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
        sprite.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        sprite.scale(1.05f, 1.05f);
    }
}

sf::Vector2f
SingleView::mousePositionInSprite(sf::Sprite& sprite)
{
    return sf::Vector2f(sf::Mouse::getPosition(window).x - sprite.getGlobalBounds().left,
            sf::Mouse::getPosition(window).y - sprite.getGlobalBounds().top);
}

void
SingleView::original(sf::Sprite& sprite)
{
    sprite.setScale(1.f, 1.f);
    const sf::Vector2u& size = sprite.getTexture()->getSize();
    sprite.setOrigin(size.x / 2, size.y / 2);
    sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}
