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
    assignToRow();
}

void
ScrollView::assignToRow()
{
    auto maxRowWidth = window.getView().getSize().x;
    std::vector<std::vector<sf::Sprite>> rows;
    rows.push_back(std::vector<sf::Sprite>());
    int index = 0, rowWidth = 0;

    for (ImageCache::ImageIter iter = imageCache.begin(); iter != imageCache.end(); iter++)
    {
        sf::Sprite& sprite = (**iter).getSprite();
        if (rowWidth + sprite.getLocalBounds().width > maxRowWidth)
        {
            index++;
            rows.push_back(std::vector<sf::Sprite>());
            rowWidth = 0;
        }
        rows[index].push_back(sprite);
        rowWidth += sprite.getLocalBounds().width;
    }

    int heightOffset = 0;
    for (auto rowIter = rows.begin(); rowIter != rows.end(); rowIter++)
        heightOffset += layoutRow(*rowIter, heightOffset);

    if (imageCache.loadingComplete() && 
            heightOffset < window.getView().getSize().y / 2 + window.getView().getCenter().y)
        imageCache.loadImages(5);
}

int
ScrollView::layoutRow(std::vector<sf::Sprite>& row, int offset)
{
    int width = 0, height = 0;
    for (auto iter = row.begin(); iter != row.end(); iter++)
    {
        width += iter->getLocalBounds().width;
        height = std::max(height, (int)iter->getLocalBounds().height);
    }
    int x = (window.getView().getSize().x - width) / 2;
    for (auto iter = row.begin(); iter != row.end(); iter++)
    {
        int y = offset + (height - iter->getLocalBounds().height) / 2;
        iter->setPosition(x, y);
        x += iter->getLocalBounds().width;
        window.draw(*iter);
    }
    return height;
}
