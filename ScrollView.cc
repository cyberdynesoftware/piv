#include "ScrollView.h"
#include <iostream>

ScrollView::ScrollView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    imageCache.loadImages(5);
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
    auto rows = spreadImagesToRows();

    int heightOffset = 0;
    for (auto rowIter = rows.begin(); rowIter != rows.end(); rowIter++)
        heightOffset += layoutRow(*rowIter, heightOffset);

    if (heightOffset < window.getView().getCenter().y + window.getView().getSize().y)
        imageCache.loadImages(5);
}

std::vector<std::vector<sf::Sprite*>>
ScrollView::spreadImagesToRows()
{
    std::vector<std::vector<sf::Sprite*>> rows;
    rows.push_back(std::vector<sf::Sprite*>());

    auto maxRowWidth = window.getView().getSize().x;
    int index = 0, rowWidth = 0;

    for (ImageCache::ImageIter iter = imageCache.begin(); iter != imageCache.end(); iter++)
    {
        sf::Sprite* sprite = &(**iter).getSprite();
        if (rowWidth + sprite->getLocalBounds().width > maxRowWidth)
        {
            index++;
            rows.push_back(std::vector<sf::Sprite*>());
            rowWidth = 0;
        }
        rows[index].push_back(sprite);
        rowWidth += sprite->getLocalBounds().width;
    }

    return rows;
}

int
ScrollView::layoutRow(std::vector<sf::Sprite*>& row, int offset)
{
    int width = 0, height = 0;
    for (auto iter = row.begin(); iter != row.end(); iter++)
    {
        width += (**iter).getLocalBounds().width;
        height = std::max(height, (int)(**iter).getLocalBounds().height);
    }
    int x = (window.getView().getSize().x - width) / 2;
    for (auto iter = row.begin(); iter != row.end(); iter++)
    {
        (**iter).setScale(1, 1);
        (**iter).setOrigin(0, 0);
        int y = offset + (height - (**iter).getLocalBounds().height) / 2;
        (**iter).setPosition(x, y);
        x += (**iter).getLocalBounds().width;
        window.draw(**iter);
    }

    return height;
}

void
ScrollView::selectImage()
{
    imageCache.currentImage = imageCache.begin();

    auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto iter = imageCache.begin(); iter != imageCache.end(); iter++)
        if ((**iter).getSprite().getGlobalBounds().contains(mouse.x, mouse.y))
            imageCache.currentImage = iter;
}

void
ScrollView::scrollToCurrentImage()
{
    // TODO
}
