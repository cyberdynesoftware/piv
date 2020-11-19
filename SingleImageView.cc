#include "SingleImageView.h"
#include <iostream>

SingleImageView::SingleImageView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
    arrow.loadFromSystem(sf::Cursor::Arrow);
    cross.loadFromSystem(sf::Cursor::Cross);
    previousMousePosition = sf::Mouse::getPosition();
}

void
SingleImageView::initImage()
{
    window.setTitle(Folder::filename(*folder.currentItem) + " - piv");

    if (image != nullptr) delete image;
    image = new Image(*folder.currentItem);
    image->fitTo(window.getSize());

    image->info.append("[");
    image->info.append(std::to_string(folder.currentItem - folder.cbegin() + 1));
    image->info.append(" / ").append(std::to_string(folder.size())).append("]");
}

void
SingleImageView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Space:
                    next();
                    break;
                case sf::Keyboard::Backspace:
                    previous();
                    break;
                case sf::Keyboard::I:
                    showInfo = (showInfo) ? false : true;
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
                case sf::Mouse::Button::Middle:
                    image->fitTo(window.getSize());
                    break;
                case sf::Mouse::XButton1:
                    next();
                    break;
                case sf::Mouse::XButton2:
                    previous();
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

                    if (sf::Mouse::getPosition().x == 0)
                        previous();
                    else if (sf::Mouse::getPosition().x == (int)window.getSize().x - 1)
                        next();

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
SingleImageView::next()
{
    if (++folder.currentItem == folder.cend())
        folder.currentItem = folder.cbegin();
    initImage();
}

void
SingleImageView::previous()
{
    if (folder.currentItem != folder.cbegin())
        folder.currentItem--;
    else
        folder.currentItem = --folder.cend();
    initImage();
}

void
SingleImageView::draw()
{
    if (image->ready)
    {
        image->update();
        window.draw(image->sprite);
    }

    if (showInfo)
    {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(16);
        text.setPosition(0, 0);
        text.setString(image->info);

        sf::FloatRect bounds = text.getLocalBounds();
        sf::Vector2f size(bounds.left * 2 + bounds.width, bounds.top * 2 + bounds.height);
        sf::RectangleShape background(size);
        background.setFillColor(sf::Color(0, 0, 0, 64));
        background.setPosition(0, 0);
        window.draw(background);

        window.draw(text);
    }
}

void
SingleImageView::resizeEvent()
{
    window.setTitle(Folder::filename(*folder.currentItem) + " - piv");
    image->fitTo(window.getSize());
}

void
SingleImageView::zoom(sf::Sprite& sprite, float delta)
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
SingleImageView::mousePositionInSprite(sf::Sprite& sprite)
{
    return sf::Vector2f(sf::Mouse::getPosition(window).x - sprite.getGlobalBounds().left,
            sf::Mouse::getPosition(window).y - sprite.getGlobalBounds().top);
}

void
SingleImageView::original(sf::Sprite& sprite)
{
    sprite.setScale(1.f, 1.f);
    const sf::Vector2u& size = sprite.getTexture()->getSize();
    sprite.setOrigin(size.x / 2, size.y / 2);
    sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}
