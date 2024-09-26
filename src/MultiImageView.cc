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
    camera.view = window.getView();

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
                    selectImage();
                    break;
                default:
                    camera.process(event);
                    break;
            }
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::A:
                    selectImage();
                    break;
                case sf::Keyboard::S:
                    {
                        showSelection = !showSelection;

                        if (showSelection)
                        {
                            lastViewPosition = camera.view.getCenter().y;
                            relayoutImages(numberOfColumns);
                            camera.setPosition(window.getView().getSize().y / 2);
                        }
                        else
                        {
                            relayoutImages(numberOfColumns);
                            camera.setPosition(lastViewPosition);
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
                        camera.setPosition(lastViewPosition);
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
                    camera.process(event);
                    break;
            }
            break;

        default:
            camera.process(event);
            break;
    }
}

std::deque<std::unique_ptr<Image>>::iterator
MultiImageView::findImageUnderMouse() const
{
    window.setView(camera.view);
    auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto imageIter = imageManager.images.begin();
            imageIter != imageManager.images.end();
            imageIter++)
    {
        if (showSelection && !(*imageIter)->selected)
        {
            continue;
        }
        if ((*imageIter)->sprite.getGlobalBounds().contains(mouseCoords))
        {
            return imageIter;
        }
    }

    return imageManager.images.end();
}

void 
MultiImageView::selectImage()
{
    auto image = findImageUnderMouse();
    if (image != imageManager.images.end())
    {
        (*image)->selected = !(*image)->selected;
    }
}

void
MultiImageView::relayoutImages(int columns)
{
    numberOfColumns = columns;

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    camera.bottom = 0;
    columnIndex = 0;
    columnOffsets.resize(numberOfColumns);
    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] = 0;

    for (auto& image : imageManager.images)
        if (image->hasPosition)
            if (!showSelection || image->selected)
                layout(image);

    lastViewPosition *= factor * factor;
    camera.setPosition(camera.view.getCenter().y * factor * factor);
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

    if (image->position.y + imageHeight > camera.bottom)
        camera.bottom = image->position.y + imageHeight;
}

void
MultiImageView::draw()
{
    window.setView(camera.view);

    for (auto& image : imageManager.images)
    {
        if (showSelection && !image->selected) continue;

        if (!image->hasPosition) layout(image);

        if (camera.isVisible(image))
        {
            window.draw(image->sprite);

            if (!showSelection && image->selected)
            {
                markSelectedImage(image);
            }

            if (showInfo)
            {
                setupInfoBox(image);
                window.draw(info);
            }

            lastVisibleImage = &image;
        }
    }

    if (camera.getBottom() > *std::ranges::min_element(columnOffsets) && !showSelection)
    {
        imageManager.loadImages(numberOfColumns);
    }

    if (camera.update())
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
MultiImageView::resize(float width, float height)
{
    camera.view.setSize(width, height);

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    camera.view.setCenter(window.getSize().x / 2, camera.view.getCenter().y * factor);
    lastViewPosition *= factor;

    for (auto& image : imageManager.images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->scaleTo(newTargetImageWidth);
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
    {
        columnOffsets[i] *= factor;
    }

    camera.bottom *= factor;
}

void
MultiImageView::calcProgress()
{
    int index = std::distance(imageManager.images.cbegin(),
            std::find(imageManager.images.cbegin(), imageManager.images.cend(), *lastVisibleImage));

    int max = !showSelection ? imageManager.numberOfFiles() :
        std::ranges::count_if(imageManager.images, 
                [](const std::unique_ptr<Image>& image) { return image->selected; });

    gui.drawProgressBar(index, max);
}
