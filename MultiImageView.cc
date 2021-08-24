#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>
#include "font.h"

MultiImageView::MultiImageView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window),
    columnOffsets(numberOfColumns, 0)
{
    folderIter = folder.cbegin();
    targetImageWidth = window.getSize().x / numberOfColumns;
    viewPosition = window.getView().getCenter().y;
    maxScrollSpeed = window.getView().getSize().y / 50;
    font.loadFromMemory(font_ttf, font_ttf_len);
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
            scrollSpeed = -event.mouseWheelScroll.delta * maxScrollSpeed;
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                    scrollSpeed = -maxScrollSpeed;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    scrollSpeed = maxScrollSpeed;
                    break;
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                    scrollSpeed = -maxScrollSpeed * 2;
                    break;
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollSpeed = maxScrollSpeed * 2;
                    break;
                case sf::Keyboard::I:
                    showInfo = (showInfo) ? false : true;
                    break;
                case sf::Keyboard::Num1:
                    numberOfColumns = 1;
                    resize(true);
                    break;
                case sf::Keyboard::Num2:
                    numberOfColumns = 2;
                    resize(true);
                    break;
                case sf::Keyboard::Num3:
                    numberOfColumns = 3;
                    resize(true);
                    break;
                case sf::Keyboard::Num4:
                    numberOfColumns = 4;
                    resize(true);
                    break;
                case sf::Keyboard::Num5:
                    numberOfColumns = 5;
                    resize(true);
                    break;
                case sf::Keyboard::Num6:
                    numberOfColumns = 6;
                    resize(true);
                    break;
                case sf::Keyboard::Num7:
                    numberOfColumns = 7;
                    resize(true);
                    break;
                case sf::Keyboard::Num8:
                    numberOfColumns = 8;
                    resize(true);
                    break;
                case sf::Keyboard::Num9:
                    numberOfColumns = 9;
                    resize(true);
                    break;
                case sf::Keyboard::Num0:
                    numberOfColumns = 10;
                    resize(true);
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
MultiImageView::scrollView(int delta)
{
    sf::View view = window.getView();
    view.move(0, delta);
    window.setView(view);
    viewPosition = view.getCenter().y;
}

void
MultiImageView::draw()
{
    bool allImagesAreReady = true;

    for (auto image : images)
    {
        if (image->ready)
        {
            if (!image->valid) continue;

            if (!image->hasPosition) layout(image);

            if (visible(image))
            {
                image->update();
                window.draw(image->sprite);
                if (showInfo) drawInfoBox(image);
            }
        }
        else
        {
            allImagesAreReady = false;
            break;
        }

    }

    if (allImagesAreReady && viewBottom() > columnOffsets[minColumnIndex()])
        loadImageRow();

    if (scrollSpeed != 0) scrollView(scrollSpeed);

    if (scrollSpeed > 0) scrollSpeed--;
    else if (scrollSpeed < 0) scrollSpeed++;
}

void
MultiImageView::resize(bool relayout)
{
    maxScrollSpeed = window.getView().getSize().y / 50;
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    if (relayout)
    {
        columnOffsets.resize(numberOfColumns);
        for (int i = 0; i < numberOfColumns; i++)
            columnOffsets[i] = 0;

        for (auto image : images)
            layout(image);
    }
    else
    {
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
    }

    viewPosition *= factor;
    sf::View view = window.getView();
    view.setCenter(view.getCenter().x, viewPosition);
    window.setView(view);
}

float
MultiImageView::viewTop()
{
    return window.getView().getCenter().y - window.getView().getSize().y;
}

float
MultiImageView::viewBottom()
{
    return window.getView().getCenter().y + window.getView().getSize().y;
}

int
MultiImageView::minColumnIndex()
{
    int columnIndex = 0;
    int minColumnOffset = columnOffsets[columnIndex];
    for (int i = 0; i < numberOfColumns; i++)
    {
        if (columnOffsets[i] < minColumnOffset)
        {
            minColumnOffset = columnOffsets[i];
            columnIndex = i;
        }
    }
    return columnIndex;
}

void
MultiImageView::layout(Image* image)
{
    const sf::Vector2u& imageSize = image->sprite.getTexture()->getSize();
    float scale = (float)targetImageWidth / imageSize.x;
    image->sprite.setScale(scale, scale);
    image->sprite.setOrigin(0, 0);
    int columnIndex = minColumnIndex();
    image->setPosition(sf::Vector2f(targetImageWidth * columnIndex, columnOffsets[columnIndex]));
    columnOffsets[columnIndex] += imageSize.y * scale;
}

void
MultiImageView::drawInfoBox(Image* image)
{
    sf::Text info;
    info.setFont(font);
    info.setFillColor(sf::Color::White);
    info.setCharacterSize(16);
    info.setPosition(image->position);
    info.setString(image->info);
    
    sf::FloatRect bounds = info.getLocalBounds();
    sf::RectangleShape background(sf::Vector2f(targetImageWidth, bounds.top * 2 + bounds.height));
    background.setFillColor(sf::Color(0, 0, 0, 64));
    background.setPosition(image->position);

    window.draw(background);
    window.draw(info);
}

bool
MultiImageView::visible(Image* image)
{
    return (image->position.y + image->sprite.getTexture()->getSize().y > viewTop() &&
            image->position.y < viewBottom());
}
