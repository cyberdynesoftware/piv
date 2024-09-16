#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>

MultiImageView::MultiImageView(sf::RenderWindow& window, ImageManager& imageManager, GUI& gui):
    window(window),
    imageManager(imageManager),
    gui(gui),
    columnOffsets(numberOfColumns, 0)
{
    targetImageWidth = window.getSize().x / numberOfColumns;
    imageManager.loadImages(numberOfColumns);
    view.it = window.getView();

    selectedImageForeground.setFillColor(sf::Color(255, 255, 255, 96));

    selectedIconCircle.setRadius(5);
    selectedIconCircle.setFillColor(pumpkin);
    selectedIconCircle.setOutlineColor(sf::Color::Black);
    selectedIconCircle.setOutlineThickness(1);
}

void
MultiImageView::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Middle:
                    {
                        auto& image = findImageUnderMouse();
                        image->selected = !image->selected;
                    }
                    break;
                default:
                    view.process(event);
                    break;
            }
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::A:
                    {
                        auto& image = findImageUnderMouse();
                        image->selected = !image->selected;
                    }
                    break;
                case sf::Keyboard::S:
                    {
                        showSelection = !showSelection;

                        if (showSelection)
                        {
                            lastViewPosition = view.it.getCenter().y;
                            relayoutImages(numberOfColumns);
                            view.setPosition(window.getView().getSize().y / 2);
                        }
                        else
                        {
                            relayoutImages(numberOfColumns);
                            view.setPosition(lastViewPosition);
                        }
                    }
                    break;
                case sf::Keyboard::C:
                    {
                        for (auto& image : imageManager.images)
                        {
                            image->selected = false;
                        }
                        showSelection = false;
                        relayoutImages(numberOfColumns);
                    }
                    break;
                case sf::Keyboard::Y:
                    imageManager.copySelectedImages();
                    break;
                case sf::Keyboard::X:
                    {
                        imageManager.moveSelectedImages();
                        showSelection = false;
                        relayoutImages(numberOfColumns);
                        view.setPosition(lastViewPosition);
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
                    view.process(event);
                    break;
            }
            break;

        default:
            view.process(event);
            break;
    }
}

std::unique_ptr<Image>&
MultiImageView::findImageUnderMouse() const
{
    window.setView(view.it);
    auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto& image : imageManager.images)
    {
        if (showSelection && !image->selected) continue;
        if (image->sprite.getGlobalBounds().contains(mouseCoords))
            return image;
    }

    //return nullptr;
}

void
MultiImageView::relayoutImages(int columns)
{
    numberOfColumns = columns;

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    view.bottom = 0;
    columnIndex = 0;
    columnOffsets.resize(numberOfColumns);
    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] = 0;

    for (auto& image : imageManager.images)
        if (image->hasPosition)
            if (!showSelection || image->selected)
                layout(image);

    lastViewPosition *= factor * factor;
    view.setPosition(view.it.getCenter().y * factor * factor);
}

void
MultiImageView::layout(std::unique_ptr<Image>& image)
{
    image->sprite.setOrigin(0, 0);
    image->scaleTo(targetImageWidth);
    int imageHeight = image->sprite.getGlobalBounds().height;

    for (int i = columnIndex + 1; i < numberOfColumns + columnIndex; i++)
    {
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

    if (image->position.y + imageHeight > view.bottom)
        view.bottom = image->position.y + imageHeight;
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
    window.setView(view.it);

    for (auto& image : imageManager.images)
    {
        if (showSelection && !image->selected) continue;

        if (!image->hasPosition) layout(image);

        if (view.isVisible(image))
        {
            window.draw(image->sprite);
            if (!showSelection && image->selected) markSelectedImage(image);

            if (showInfo)
            {
                setupInfoBox(image);
                window.draw(info);
            }
            lastVisibleImage = &image;
        }
    }

    if (view.getBottom() > columnOffsets[minColumnIndex()] && !showSelection)
    {
        imageManager.loadImages(numberOfColumns);
    }

    if (view.update())
    {
        calcProgress();
    }
}

void
MultiImageView::markSelectedImage(const std::unique_ptr<Image>& image)
{
    selectedImageForeground.setPosition(image->sprite.getGlobalBounds().getPosition());
    selectedImageForeground.setSize(image->sprite.getGlobalBounds().getSize());
    window.draw(selectedImageForeground);

    const auto& imageBounds = image->sprite.getGlobalBounds();
    selectedIconCircle.setPosition(imageBounds.left + imageBounds.width - 20, imageBounds.top + 8);
    window.draw(selectedIconCircle);
}

void
MultiImageView::resize()
{
    //highlightBackground.setSize(view.getSize());
    //highlightBackground.setOrigin(view.getSize() / 2.f);

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    view.setPosition(view.it.getCenter().y * factor);
    lastViewPosition *= factor;

    for (auto& image : imageManager.images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->sprite.scale(factor, factor);
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
    {
        columnOffsets[i] *= factor;
    }

    view.bottom *= factor;
}

void
MultiImageView::calcProgress()
{
    int index = std::distance(imageManager.images.cbegin(),
            std::find(imageManager.images.cbegin(), imageManager.images.cend(), *lastVisibleImage));

    int max = !showSelection ? imageManager.numberOfFiles() :
        std::count_if(imageManager.images.cbegin(), imageManager.images.cend(), 
                [](const std::unique_ptr<Image>& image) { return image->selected; });

    gui.drawProgressBar(index, max);
}
