#include "ScrollView.h"
#include <iostream>

ScrollView::ScrollView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    imageCache.loadImages(numberOfColumns * 3);
}

void
ScrollView::handle(sf::Event& event)
{
    auto view = window.getView();
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.delta < 0)
                view.move(0, 10);
            else if (event.mouseWheelScroll.delta > 0)
                view.move(0, -10);
            window.setView(view);
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                    view.move(0, -100);
                    window.setView(view);
                    break;
                case sf::Keyboard::Down:
                    view.move(0, 100);
                    window.setView(view);
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
ScrollView::draw()
{
    int size = window.getView().getSize().x / numberOfColumns;
    int heightOffset = 0;
    int column = 0;

    for (auto iter = imageCache.begin(); iter != imageCache.end(); iter++)
    {
        sf::Sprite& sprite = (**iter).getSquareSprite();
        float factor = size / sprite.getLocalBounds().width;
        sprite.setScale(factor, factor);
        sprite.setPosition(size * column, heightOffset);
        window.draw(sprite);

        column++;
        if (column == numberOfColumns)
        {
            heightOffset += size;
            column = 0;
        }
    }

    if (heightOffset < window.getView().getCenter().y + window.getView().getSize().y)
        imageCache.loadImages(numberOfColumns);
}

void
ScrollView::selectImage()
{
    imageCache.currentImage = imageCache.begin();

    auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto iter = imageCache.begin(); iter != imageCache.end(); iter++)
        if ((**iter).getSquareSprite().getGlobalBounds().contains(mouse.x, mouse.y))
            imageCache.currentImage = iter;
}

void
ScrollView::scrollToCurrentImage()
{
    auto bounds = (**imageCache.currentImage).getSquareSprite().getGlobalBounds();
    auto view = window.getView();
    view.setCenter(view.getCenter().x, bounds.top + bounds.height / 2);
    window.setView(view);
}
