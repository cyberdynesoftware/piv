#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>

MultiImageView::MultiImageView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window),
    columnOffsets(numberOfColumns, 0)
{
    folderIter = folder.cbegin();
    targetImageWidth = window.getSize().x / numberOfColumns;
    viewPosition = window.getView().getCenter().y;
    loadImageRow();
}

void
MultiImageView::loadImageRow()
{
    for (int i = 0; i < numberOfColumns; i++)
    {
        if (folderIter == folder.cend()) break;
        images.push_back(new Image(*folderIter++));
    }
}

void
MultiImageView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            scrollSpeed = event.mouseWheelScroll.delta * -25;
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    break;
                case sf::Keyboard::I:
                    showInfo = (showInfo) ? false : true;
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
MultiImageView::scroll(int delta)
{
    sf::View view = window.getView();
    view.move(0, delta * -10);
    window.setView(view);
}

void
MultiImageView::draw()
{
    bool allImagesAreReady = true;
    unsigned int minColumnOffset;

    for (auto image : images)
    {
        int columnIndex = 0;
        minColumnOffset = columnOffsets[columnIndex];
        for (int i = 0; i < numberOfColumns; i++)
        {
            if (columnOffsets[i] < minColumnOffset)
            {
                minColumnOffset = columnOffsets[i];
                columnIndex = i;
            }
        }

        if (image->ready)
        {
            image->update();
            if (!image->hasPosition)
            {
                const sf::Vector2u& imageSize = image->sprite.getTexture()->getSize();
                float scale = (float)targetImageWidth / imageSize.x;
                image->sprite.setScale(scale, scale);
                image->sprite.setOrigin(0, 0);
                image->setPosition(sf::Vector2f(targetImageWidth * columnIndex, minColumnOffset));
                columnOffsets[columnIndex] += imageSize.y * scale;
            }
            window.draw(image->sprite);
        }
        else
        {
            allImagesAreReady = false;
            break;
        }

        if (showInfo)
        {
            sf::RectangleShape background(sf::Vector2f(targetImageWidth, 20));
            background.setFillColor(sf::Color(0, 0, 0, 64));
            background.setPosition(image->position);
            window.draw(background);

            sf::Text info;
            info.setFont(font);
            info.setFillColor(sf::Color::White);
            info.setCharacterSize(16);
            info.setPosition(image->position);
            info.setString(Folder::filename(image->path));
            window.draw(info);
        }
    }

    if (allImagesAreReady && viewBottom() > minColumnOffset)
        loadImageRow();

    if (scrollSpeed != 0)
    {
        sf::View view = window.getView();
        view.move(0, scrollSpeed);
        window.setView(view);
        viewPosition = view.getCenter().y;
    }

    if (scrollSpeed > 0) scrollSpeed--;
    else if (scrollSpeed < 0) scrollSpeed++;
}

void
MultiImageView::resize()
{
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    for (auto image : images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->sprite.getPosition() * factor);
            image->sprite.scale(factor, factor);
        }
    }
    
    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] *= factor;

    viewPosition *= factor;

    sf::View view = window.getView();
    view.setCenter(view.getCenter().x, viewPosition);
    window.setView(view);
}

float
MultiImageView::viewBottom()
{
    return window.getView().getCenter().y + window.getView().getSize().y;
}
