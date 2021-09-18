#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>
#include "font.h"
#include "Help.h"

MultiImageView::MultiImageView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window),
    columnOffsets(numberOfColumns, 0)
{
    folderIter = folder.cbegin();
    targetImageWidth = window.getSize().x / numberOfColumns;
    viewPosition = lastViewPosition = window.getView().getCenter().y;
    viewHeight = window.getView().getSize().y;
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
            setViewPosition(viewPosition - event.mouseWheelScroll.delta * viewHeight / 100);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::XButton1:
                    scrollState = DOWN;
                    break;
                case sf::Mouse::Button::XButton2:
                    scrollState = UP;
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    scrollState = NONE;
                    pickImage();
                    break;
                case sf::Mouse::Button::Right:
                    unpickImage();
                    break;
                case sf::Mouse::Button::XButton1:
                case sf::Mouse::Button::XButton2:
                    scrollState = NONE;
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
                    unpickImage();
                    break;
                case sf::Keyboard::A:
                    if (elevatedImage != NULL)
                        elevatedImage->selected = true;
                    break;
                case sf::Keyboard::S:
                    if (elevatedImage == NULL)
                    {
                        showSelection = !showSelection;

                        if (showSelection)
                        {
                            lastViewPosition = viewPosition;
                            relayoutImages(numberOfColumns);
                            setViewPosition(window.getView().getSize().y / 2);
                        }
                        else
                        {
                            relayoutImages(numberOfColumns);
                            setViewPosition(lastViewPosition);
                        }
                    }
                    break;
                case sf::Keyboard::C:
                    if (elevatedImage == NULL)
                    {
                        for (auto image : images)
                            image->selected = false;

                        showSelection = false;
                        relayoutImages(numberOfColumns);
                    }
                    else
                    {
                        elevatedImage->selected = false;
                    }
                    break;
                case sf::Keyboard::Space:
                    scrollState = (scrollState == NONE) ? AUTO_SCROLL : NONE;
                    break;
                case sf::Keyboard::Home:
                case sf::Keyboard::G:
                    setViewPosition(window.getView().getSize().y / 2);
                    scrollSpeed = 0;
                    break;
                case sf::Keyboard::H:
                    showHelp = true;
                    break;
                case sf::Keyboard::Num1:
                    relayoutImages(1);
                    break;
                case sf::Keyboard::Num2:
                    relayoutImages(2);
                    break;
                case sf::Keyboard::Num3:
                    relayoutImages(3);
                    break;
                case sf::Keyboard::Num4:
                    relayoutImages(4);
                    break;
                case sf::Keyboard::Num5:
                    relayoutImages(5);
                    break;
                case sf::Keyboard::Num6:
                    relayoutImages(6);
                    break;
                case sf::Keyboard::Num7:
                    relayoutImages(7);
                    break;
                case sf::Keyboard::Num8:
                    relayoutImages(8);
                    break;
                case sf::Keyboard::Num9:
                    relayoutImages(9);
                    break;
                case sf::Keyboard::Num0:
                    relayoutImages(10);
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
                case sf::Keyboard::H:
                    showHelp = false;
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
MultiImageView::setViewPosition(int centerY)
{
    sf::View view = window.getView();
    view.setCenter(view.getCenter().x, centerY);

    if (view.getCenter().y - view.getSize().y / 2 < 0 || bottom < view.getSize().y)
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    else if (folderIter == folder.cend() && view.getCenter().y + view.getSize().y / 2 > bottom)
        view.setCenter(view.getCenter().x, bottom - view.getSize().y / 2);

    window.setView(view);
    viewPosition = view.getCenter().y;
}

void
MultiImageView::pickImage()
{
    if (elevatedImage == NULL)
    {
        bool imageFound = false;
        auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto image : images)
        {
            if (showSelection && !image->selected) continue;
            if (image->sprite.getGlobalBounds().contains(mouseCoords.x, mouseCoords.y))
            {
                elevatedImage = image;
                elevatedImage->fitTo(window.getView());
                imageFound = true;
                break;
            }
        }

        if (imageFound)
            for (auto image : images)
                if (image != elevatedImage && isVisible(image))
                    image->sprite.setColor(sf::Color(255, 255, 255, 31));
    }
}

void
MultiImageView::unpickImage()
{
    if (elevatedImage != NULL)
    {
        const sf::Vector2u& imageSize = elevatedImage->sprite.getTexture()->getSize();
        float scale = (float)targetImageWidth / imageSize.x;
        elevatedImage->sprite.setScale(scale, scale);
        elevatedImage->sprite.setOrigin(0, 0);
        elevatedImage->sprite.setPosition(elevatedImage->position);
        elevatedImage = NULL;

        for (auto image : images)
            if (image != elevatedImage && isVisible(image))
                image->sprite.setColor(sf::Color::White);

        scrollSpeed = 0;
    }
}

void
MultiImageView::relayoutImages(int columns)
{
    if (elevatedImage == NULL)
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
                if (!showSelection || image->selected)
                    layout(image);

        lastViewPosition *= factor * factor;
        setViewPosition(viewPosition * factor * factor);
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
MultiImageView::draw()
{
    bool allImagesAreReady = true;

    for (auto image : images)
    {
        if (image->ready)
        {
            if (!image->valid) continue;

            if (image == elevatedImage) continue;

            if (showSelection && !image->selected) continue;

            if (!image->hasPosition)
                layout(image);

            if (isVisible(image))
            {
                image->update();

                window.draw(image->sprite);

                if (showInfo && elevatedImage == NULL)
                    drawInfoBox(image);

                if (!showSelection && elevatedImage == NULL && image->selected)
                    drawSelectedIcon(image);

                lastVisibleImage = image;
            }
        }
        else
        {
            allImagesAreReady = false;
            break;
        }
    }

    if (elevatedImage != NULL)
    {
        elevatedImage->update();
        window.draw(elevatedImage->sprite);
        if (showInfo) drawInfoBox(elevatedImage);
        if (!showSelection && elevatedImage->selected) drawSelectedIcon(elevatedImage);
    }

    if (allImagesAreReady && viewPosition + viewHeight / 2 > columnOffsets[minColumnIndex()])
        loadImageRow();

    if (showHelp)
        drawHelpText();

    scrollView();
}

bool
MultiImageView::isVisible(Image* image)
{
    const auto& bounds = image->sprite.getGlobalBounds();
    return (bounds.top + bounds.height > viewPosition - viewHeight / 2 &&
            bounds.top < viewPosition + viewHeight / 2);
}

void
MultiImageView::drawInfoBox(Image* image)
{
    sf::Text info;
    info.setFont(font);
    info.setFillColor(sf::Color::White);
    info.setCharacterSize(15);
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

void
MultiImageView::drawSelectedIcon(Image* image)
{
    sf::CircleShape circle;
    circle.setRadius(4);
    circle.setFillColor(sf::Color::Yellow);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(3);

    const auto& imageBounds = image->sprite.getGlobalBounds();
    circle.setPosition(imageBounds.left + imageBounds.width - 16, imageBounds.top + 8);

    window.draw(circle);
}

void
MultiImageView::scrollView()
{
    if (elevatedImage == NULL)
    {
        switch (scrollState)
        {
            case UP:
                scrollSpeed = -viewHeight / 50;
                break;
            case DOWN:
                scrollSpeed = viewHeight / 50;
                break;
            case UP_FAST:
                scrollSpeed = -viewHeight / 25;
                break;
            case DOWN_FAST:
                scrollSpeed = viewHeight / 25;
                break;
            case AUTO_SCROLL:
                scrollSpeed = viewHeight / 400;
                break;
            default:
                break;
        }

        if (scrollSpeed != 0)
        {
            setViewPosition(viewPosition + scrollSpeed);

            if (scrollSpeed > 0) scrollSpeed--;
            else if (scrollSpeed < 0) scrollSpeed++;

            drawProgressBar();
        }
    }
}

void
MultiImageView::drawProgressBar()
{
    float progress = (float) std::distance(folder.cbegin(), lastVisibleImage->folderIter) / folder.size();
    progressBar.setSize(sf::Vector2f(progressBarWidth, progress * viewHeight));
    progressBar.setPosition(window.getView().getSize().x - progressBarWidth, viewPosition - viewHeight / 2);
    window.draw(progressBar);
}

void
MultiImageView::resize()
{
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    viewHeight = window.getView().getSize().y;
    setViewPosition(viewPosition * factor);
    lastViewPosition *= factor;

    for (auto image : images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->sprite.scale(factor, factor);
            if (image == elevatedImage)
                elevatedImage->fitTo(window.getView());
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] *= factor;
}

void
MultiImageView::drawHelpText()
{
    std::string help = Help::general();
    if (elevatedImage != NULL)
        help.append(Help::singleImage());
    else
        help.append(Help::allImages());

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(18);
    text.setString(help);
    text.setPosition(100, 100);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition(window.getView().getCenter());

    sf::RectangleShape background(sf::Vector2f(text.getLocalBounds().width + 50, text.getLocalBounds().height + 20));
    background.setFillColor(sf::Color(0, 0, 0, 192));
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
    background.setPosition(window.getView().getCenter());

    window.draw(background);
    window.draw(text);
}
