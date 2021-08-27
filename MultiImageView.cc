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
    yViewPosition = window.getView().getCenter().y;
    yViewSize = window.getView().getSize().y;
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
            scrollSpeed = -event.mouseWheelScroll.delta * yViewSize / 50;
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    if (selectedImage == NULL)
                    {
                        selectImage();
                        selectedImage->centerOrigin();
                        selectedImage->sprite.setPosition(window.getView().getCenter());
                        selectedImage->sprite.scale(1.25f, 1.25f);
                    }
                    break;
                case sf::Mouse::Button::Right:
                    if (selectedImage != NULL)
                    {
                        selectedImage->sprite.setOrigin(0, 0);
                        selectedImage->sprite.setPosition(selectedImage->position);
                        selectedImage->sprite.scale(.8f, .8f);
                        selectedImage = NULL;
                    }
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                    scrollSpeed = -yViewSize / 50;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    scrollSpeed = yViewSize / 50;
                    break;
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                    scrollSpeed = -yViewSize / 25;
                    break;
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollSpeed = yViewSize / 25;
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
    if (selectedImage == NULL)
    {
        sf::View view = window.getView();
        view.move(0, delta);

        if (view.getCenter().y - yViewSize / 2 < 0)
            view.setCenter(view.getCenter().x, yViewSize / 2);
        else if (folderIter == folder.cend() && 
                view.getCenter().y + yViewSize / 2 > bottom)
            view.setCenter(view.getCenter().x, bottom - yViewSize / 2);

        window.setView(view);
        yViewPosition = view.getCenter().y;
    }
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

            if (image == selectedImage) continue;

            if (!image->hasPosition) layout(image);

            if (visible(image))
            {
                image->update();

                if (selectedImage != NULL)
                    image->sprite.setColor(sf::Color(255, 255, 255, 31));
                else
                    image->sprite.setColor(sf::Color::White);

                window.draw(image->sprite);
                if (showInfo && selectedImage != NULL) drawInfoBox(image);
            }
        }
        else
        {
            allImagesAreReady = false;
            break;
        }
    }

    if (selectedImage != NULL)
    {
        selectedImage->update();
        window.draw(selectedImage->sprite);
    }

    if (allImagesAreReady && yViewPosition + yViewSize / 2 > columnOffsets[minColumnIndex()])
        loadImageRow();

    if (scrollSpeed != 0) scrollView(scrollSpeed);

    if (scrollSpeed > 0) scrollSpeed--;
    else if (scrollSpeed < 0) scrollSpeed++;
}

void
MultiImageView::resize(bool relayout)
{
    yViewSize = window.getView().getSize().y;
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    if (relayout)
    {
        bottom = 0;
        columnOffsets.resize(numberOfColumns);
        for (int i = 0; i < numberOfColumns; i++)
            columnOffsets[i] = 0;

        for (auto image : images)
            if (image->hasPosition)
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

    yViewPosition *= factor * factor;
    sf::View view = window.getView();
    view.setCenter(view.getCenter().x, yViewPosition);
    window.setView(view);
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

    if (image->position.y + imageSize.y * scale > bottom)
        bottom = image->position.y + imageSize.y * scale;
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
    return (image->position.y + image->sprite.getTexture()->getSize().y > yViewPosition - yViewSize / 2 &&
            image->position.y < yViewPosition + yViewSize / 2);
}

void
MultiImageView::selectImage()
{
    if (selectedImage == NULL)
    {
        auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto image : images)
        {
            if (image->sprite.getGlobalBounds().contains(mouseCoords.x, mouseCoords.y))
            {
                selectedImage = image;
                break;
            }
        }
    }
}
