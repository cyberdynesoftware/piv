#include "SingleImageView.h"
#include <iostream>

SingleImageView::SingleImageView(sf::RenderWindow& window, ImageManager& imageManager):
    imageManager(imageManager),
    window(window)
{
    background.setFillColor(sf::Color(0, 0, 0, 224));
    background.setSize(window.getDefaultView().getSize());

    previousMousePosition = sf::Mouse::getPosition();
}

void
SingleImageView::process(const sf::Event& event)
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
                case sf::Keyboard::A:
                    (*imageIter)->selected = !(*imageIter)->selected;
                    next();
                    break;
                case sf::Keyboard::O:
                    toggleFit();
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            zoom(event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
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
                case sf::Mouse::Button::Middle:
                    (*imageIter)->selected = !(*imageIter)->selected;
                    break;

                default:
                    break;
            }
            break;

        case sf::Event::MouseMoved:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i delta(sf::Mouse::getPosition() - previousMousePosition);
                (*imageIter)->sprite.move(delta.x, delta.y);
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
    auto& image = *imageIter;

    if (imageIter != imageManager.images.end() && ++imageIter != imageManager.images.end())
    {
        image->resetPositionAndScale();
        init();
    }
}

void
SingleImageView::previous()
{
    if (imageIter != imageManager.images.begin())
    {
        (*imageIter)->resetPositionAndScale();
        --imageIter;
        init();
    }
}

void
SingleImageView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw((*imageIter)->sprite, states);

    if (showInfo)
    {
        target.draw(info, states);
    }
}

void
SingleImageView::init()
{
    (*imageIter)->fitTo(window.getDefaultView());
    setupInfoBox(*imageIter);
}

void
SingleImageView::zoom(float delta)
{
    if (delta < 0)
    {
        (*imageIter)->sprite.scale(0.95f, 0.95f);
    }
    else if (delta > 0)
    {
        window.setView(window.getDefaultView());

        auto mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        auto mousePosSprite = mousePosition - (*imageIter)->sprite.getGlobalBounds().getPosition();

        (*imageIter)->sprite.setOrigin(mousePosSprite / (*imageIter)->sprite.getScale().x);
        (*imageIter)->sprite.setPosition(mousePosition);
        (*imageIter)->sprite.scale(1.05f, 1.05f);
    }
    setupInfoBox(*imageIter);
}

void
SingleImageView::toggleFit()
{
    if ((*imageIter)->sprite.getScale().x == 1.f)
    {
        (*imageIter)->fitTo(window.getDefaultView());
    }
    else
    {
        (*imageIter)->sprite.setScale(1.f, 1.f);
    }
    setupInfoBox(*imageIter);
}
