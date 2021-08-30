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
    progressBar.setFillColor(sf::Color(255, 255, 255, 128));
    loadImageRow();
}

void
MultiImageView::loadImageRow()
{
    for (int i = 0; i < numberOfColumns; i++)
    {
        if (folderIter == folder.cend()) break;
        images.push_back(new Image(folderIter++));
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
                    if (selectImage())
                        selectedImage->fitTo(window.getView());
                    break;
                case sf::Mouse::Button::Right:
                    unselectImage();
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
                    scrollState = UP;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    scrollState = DOWN;
                    break;
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                    scrollState = UP_FAST;
                    break;
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollState = DOWN_FAST;
                    break;
                case sf::Keyboard::I:
                    showInfo = (showInfo) ? false : true;
                    break;
                case sf::Keyboard::M:
                    unselectImage();
                    break;
                case sf::Keyboard::Num1:
                    relayout(1);
                    break;
                case sf::Keyboard::Num2:
                    relayout(2);
                    break;
                case sf::Keyboard::Num3:
                    relayout(3);
                    break;
                case sf::Keyboard::Num4:
                    relayout(4);
                    break;
                case sf::Keyboard::Num5:
                    relayout(5);
                    break;
                case sf::Keyboard::Num6:
                    relayout(6);
                    break;
                case sf::Keyboard::Num7:
                    relayout(7);
                    break;
                case sf::Keyboard::Num8:
                    relayout(8);
                    break;
                case sf::Keyboard::Num9:
                    relayout(9);
                    break;
                case sf::Keyboard::Num0:
                    relayout(10);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                case sf::Keyboard::K:
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                case sf::Keyboard::PageUp:
                case sf::Keyboard::U:
                case sf::Keyboard::PageDown:
                case sf::Keyboard::D:
                    scrollState = NONE;
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
MultiImageView::scrollView()
{
    switch (scrollState)
    {
        case UP:
            scrollSpeed = -yViewSize / 50;
            break;
        case DOWN:
            scrollSpeed = yViewSize / 50;
            break;
        case UP_FAST:
            scrollSpeed = -yViewSize / 25;
            break;
        case DOWN_FAST:
            scrollSpeed = yViewSize / 25;
            break;
        default:
            break;
    }

    if (scrollSpeed != 0 && selectedImage == NULL)
    {
        sf::View view = window.getView();
        view.move(0, scrollSpeed);
        restrict(view);
        window.setView(view);
        yViewPosition = view.getCenter().y;

        if (scrollSpeed > 0) scrollSpeed--;
        else if (scrollSpeed < 0) scrollSpeed++;

        drawProgressBar();
    }
}

void
MultiImageView::restrict(sf::View& view)
{
    if (view.getCenter().y - view.getSize().y / 2 < 0)
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    else if (folderIter == folder.cend() && 
            view.getCenter().y + view.getSize().y / 2 > bottom)
        view.setCenter(view.getCenter().x, bottom - view.getSize().y / 2);
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
                if (showInfo && selectedImage == NULL) drawInfoBox(image);

                lastVisibleImage = image;
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
        if (showInfo) drawInfoBox(selectedImage);
    }

    if (allImagesAreReady && yViewPosition + yViewSize / 2 > columnOffsets[minColumnIndex()])
        loadImageRow();

    scrollView();
}

void
MultiImageView::resize()
{
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    yViewSize = window.getView().getSize().y;
    yViewPosition *= factor;

    sf::View view = window.getView();
    view.setCenter(view.getCenter().x, yViewPosition);
    restrict(view);
    window.setView(view);

    for (auto image : images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->sprite.scale(factor, factor);
            if (image == selectedImage)
                selectedImage->fitTo(window.getView());
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] *= factor;
}

void
MultiImageView::relayout(int columns)
{
    if (selectedImage == NULL)
    {
        numberOfColumns = columns;

        int newTargetImageWidth = window.getSize().x / numberOfColumns;
        float factor = (float) newTargetImageWidth / targetImageWidth;
        targetImageWidth = newTargetImageWidth;

        bottom = 0;
        columnOffsets.resize(numberOfColumns);
        for (int i = 0; i < numberOfColumns; i++)
            columnOffsets[i] = 0;

        for (auto image : images)
            if (image->hasPosition)
                layout(image);

        yViewPosition *= factor * factor;
        sf::View view = window.getView();
        view.setCenter(view.getCenter().x, yViewPosition);
        restrict(view);
        window.setView(view);
    }
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
MultiImageView::unselectImage()
{
    if (selectedImage != NULL)
    {
        const sf::Vector2u& imageSize = selectedImage->sprite.getTexture()->getSize();
        float scale = (float)targetImageWidth / imageSize.x;
        selectedImage->sprite.setScale(scale, scale);
        selectedImage->sprite.setOrigin(0, 0);
        selectedImage->sprite.setPosition(selectedImage->position);
        selectedImage = NULL;
    }
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
    info.setString(image->info);
    
    const auto& infoBounds = info.getLocalBounds();
    const auto& imageBounds = image->sprite.getGlobalBounds();
    sf::RectangleShape background(sf::Vector2f(imageBounds.width, infoBounds.top * 2 + infoBounds.height));
    background.setFillColor(sf::Color(0, 0, 0, 64));

    background.setPosition(imageBounds.left, imageBounds.top);
    info.setPosition(imageBounds.left, imageBounds.top);

    window.draw(background);
    window.draw(info);
}

bool
MultiImageView::visible(Image* image)
{
    const auto& bounds = image->sprite.getGlobalBounds();
    return (bounds.top + bounds.height > yViewPosition - yViewSize / 2 &&
            bounds.top < yViewPosition + yViewSize / 2);
}

bool
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
                return true;
            }
        }
    }
    return false;
}

void
MultiImageView::drawProgressBar()
{
    float progress = (float) std::distance(folder.cbegin(), lastVisibleImage->folderIter) / folder.size();
    progressBar.setSize(sf::Vector2f(progressBarWidth, progress * yViewSize));
    progressBar.setPosition(window.getView().getSize().x - progressBarWidth, yViewPosition - yViewSize / 2);
    window.draw(progressBar);
}
