#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>
#include <format>
#include <algorithm>

MultiImageView::MultiImageView(sf::RenderWindow& window, ImageManager& imageManager, GUI& gui):
    window(window),
    imageManager(imageManager),
    gui(gui),
    columnOffsets(numberOfColumns, 0)
{
    targetImageWidth = window.getSize().x / numberOfColumns;
    viewPosition = lastViewPosition = window.getView().getCenter().y;
    viewHeight = window.getView().getSize().y;
    highlightBackground.setFillColor(sf::Color(0, 0, 0, 224));
    highlightBackground.setSize(window.getView().getSize());
    highlightBackground.setOrigin(window.getView().getSize() / 2.f);
    imageManager.loadImages(numberOfColumns);
    view = window.getView();
}

void
MultiImageView::process(const sf::Event& event)
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
                case sf::Mouse::Button::XButton1:
                case sf::Mouse::Button::XButton2:
                    scrollState = NONE;
                    break;
                case sf::Mouse::Button::Middle:
                    {
                        Image* image = findImageUnderMouse();
                        image->selected = !image->selected;
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
                case sf::Keyboard::A:
                    {
                        Image* image = findImageUnderMouse();
                        image->selected = !image->selected;
                    }
                    break;
                case sf::Keyboard::S:
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

                        //gui.helpMsg(generateHelpText());
                    }
                    break;
                case sf::Keyboard::C:
                    {
                        for (auto image : imageManager.images)
                            image->selected = false;

                        showSelection = false;
                        relayoutImages(numberOfColumns);
                    }
                    break;
                case sf::Keyboard::Space:
                    scrollState = (scrollState == NONE) ? AUTO_SCROLL : NONE;
                    break;
                case sf::Keyboard::Home:
                case sf::Keyboard::G:
                    setViewPosition(view.getSize().y / 2);
                    scrollSpeed = 0;
                    break;
                case sf::Keyboard::Y:
                    imageManager.copySelectedImages();
                    break;
                case sf::Keyboard::X:
                    {
                        imageManager.moveSelectedImages();
                        showSelection = false;
                        relayoutImages(numberOfColumns);
                        setViewPosition(lastViewPosition);
                    }
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
    view.setCenter(view.getCenter().x, centerY);

    if (view.getCenter().y - view.getSize().y / 2 < 0 || bottom < view.getSize().y)
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    else if (/*(folderIter == folder.cend() || showSelection) &&*/ view.getCenter().y + view.getSize().y / 2 > bottom)
        view.setCenter(view.getCenter().x, bottom - view.getSize().y / 2);

    viewPosition = view.getCenter().y;
}

Image*
MultiImageView::findImageUnderMouse()
{
    window.setView(view);
    auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto image : imageManager.images)
    {
        if (showSelection && !image->selected) continue;
        if (image->sprite.getGlobalBounds().contains(mouseCoords))
            return image;
    }

    return NULL;
}

void
MultiImageView::relayoutImages(int columns)
{
    numberOfColumns = columns;

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    bottom = 0;
    columnIndex = 0;
    columnOffsets.resize(numberOfColumns);
    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] = 0;

    for (auto image : imageManager.images)
        if (image->hasPosition)
            if (!showSelection || image->selected)
                layout(image);

    lastViewPosition *= factor * factor;
    setViewPosition(viewPosition * factor * factor);
}

void
MultiImageView::layout(Image* image)
{
    image->sprite.setOrigin(0, 0);
    image->scaleTo(targetImageWidth);
    int imageHeight = image->sprite.getGlobalBounds().height;

    for (int i = columnIndex + 1; i < numberOfColumns + columnIndex; i++) {
        int halfImageHeight = (i < numberOfColumns) ? imageHeight / 2 : -imageHeight / 2;
        int index = i % numberOfColumns;
        if (columnOffsets[index] + halfImageHeight <= columnOffsets[columnIndex])
        {
            columnIndex = index;
            break;
        }
    }

    image->setPosition(sf::Vector2f(targetImageWidth * columnIndex, columnOffsets[columnIndex]));
    columnOffsets[columnIndex] += imageHeight;

    if (image->position.y + imageHeight > bottom)
        bottom = image->position.y + imageHeight;
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
    window.setView(view);

    for (auto image : imageManager.images)
    {
        //if (image == elevatedImage) continue;
        if (showSelection && !image->selected) continue;
        if (!image->hasPosition) layout(image);

        if (isVisible(image))
        {
            image->update();
            window.draw(image->sprite);
            if (!showSelection  && image->selected) highlight(image);
            gui.drawInfoBox(image);
            if (!showSelection  && image->selected) gui.drawSelectedIcon(image);
            lastVisibleImage = image;
        }
    }
    /*
    if (elevatedImage != NULL)
    {
        highlightBackground.setPosition(view.getCenter());
        window.draw(highlightBackground);
        elevatedImage->update();
        window.draw(elevatedImage->sprite);
        gui.drawInfoBox(elevatedImage);
        if (!showSelection  && elevatedImage->selected) gui.drawSelectedIcon(elevatedImage);
    }
    */
    if (viewPosition + viewHeight / 2 > columnOffsets[minColumnIndex()] && !showSelection)
        imageManager.loadImages(numberOfColumns);

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
MultiImageView::highlight(Image* image)
{
    const auto& imageBounds = image->sprite.getGlobalBounds();
    sf::RectangleShape highlight(sf::Vector2f(imageBounds.width, imageBounds.height));
    highlight.setPosition(imageBounds.left, imageBounds.top);
    highlight.setFillColor(sf::Color(255, 255, 255, 96));
    window.draw(highlight);
}

void
MultiImageView::scrollView()
{
    switch (scrollState)
    {
        case UP:
            scrollSpeed = viewHeight / -50;
            break;
        case DOWN:
            scrollSpeed = viewHeight / 50;
            break;
        case UP_FAST:
            scrollSpeed = viewHeight / -25;
            break;
        case DOWN_FAST:
            scrollSpeed = viewHeight / 25;
            break;
        case AUTO_SCROLL:
            scrollSpeed = viewHeight / 800;
            if (scrollSpeed == 0) scrollSpeed = 1;
            break;
        default:
            break;
    }

    if (scrollSpeed != 0)
    {
        setViewPosition(viewPosition + scrollSpeed);

        //if (scrollSpeed > 0) scrollSpeed--;
        //else if (scrollSpeed < 0) scrollSpeed++;
        scrollSpeed = scrollSpeed / 2;

        calcProgress();
    }
}

float
MultiImageView::calcProgress()
{
    int index = std::distance(imageManager.images.cbegin(),
            std::find(imageManager.images.cbegin(), imageManager.images.cend(), lastVisibleImage));

    int max = !showSelection ? imageManager.numberOfFiles() :
        std::count_if(imageManager.images.cbegin(), imageManager.images.cend(), 
                [](const Image* image) { return image->selected; });

    auto progress =  (float) index / max;
    auto msg = std::format("{} / {}", index, max);
    gui.drawProgressBar(progress, msg);

    return 1.f;
}

void
MultiImageView::resize()
{
    highlightBackground.setSize(view.getSize());
    highlightBackground.setOrigin(view.getSize() / 2.f);

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    viewHeight = view.getSize().y;
    setViewPosition(viewPosition * factor);
    lastViewPosition *= factor;

    for (auto image : imageManager.images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->sprite.scale(factor, factor);
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] *= factor;

    bottom *= factor;
}
