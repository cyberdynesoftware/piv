#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>

MultiImageView::MultiImageView(sf::RenderWindow& window, ImageManager& imageManager, Camera& camera):
    window(window),
    imageManager(imageManager),
    camera(camera),
    columnOffsets(numberOfColumns, 0)
{
    targetImageWidth = window.getSize().x / numberOfColumns;
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
                    toggleShowSelection();
                    break;
                case sf::Keyboard::C:
                    clearSelection();
                    break;
                case sf::Keyboard::O:
                    toggleSortOrder();
                    break;
                case sf::Keyboard::Y:
                    imageManager.copySelectedImages();
                    break;
                case sf::Keyboard::X:
                    moveSelectedImages();
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
MultiImageView::toggleShowSelection()
{
    showSelection = !showSelection;

    if (showSelection)
    {
        lastViewPosition = camera.getTop();
        relayoutImages(numberOfColumns);
        camera.teleport(camera.view.getSize().y / 2.f);
    }
    else
    {
        relayoutImages(numberOfColumns);
        camera.teleport(lastViewPosition);
    }
}

void
MultiImageView::clearSelection()
{
    for (auto& image : imageManager.images)
    {
        image->selected = false;
    }
    showSelection = false;
    relayoutImages(numberOfColumns);
}

void
MultiImageView::moveSelectedImages()
{
    imageManager.moveSelectedImages();
    showSelection = false;
    relayoutImages(numberOfColumns);
    camera.teleport(camera.view.getSize().y / 2.f);
}

void
MultiImageView::toggleSortOrder()
{
    auto intEnum = static_cast<int>(sortOrder);
    sortOrder = static_cast<ImageManager::SORT_ORDER>(++intEnum);

    if (sortOrder == ImageManager::SORT_ORDER::ENUM_GUARD)
    {
        sortOrder = ImageManager::SORT_ORDER::NAME;
    }

    reset();
    imageManager.changeSortOrder(sortOrder);
    camera.teleport(camera.view.getSize().y / 2.f);
}

void
MultiImageView::reset()
{
    camera.bottom = 0;
    columnIndex = 0;
    std::ranges::fill(columnOffsets, 0);
}

void
MultiImageView::relayoutImages(int columns)
{
    numberOfColumns = columns;

    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    columnOffsets.resize(numberOfColumns);
    reset();

    for (auto& image : imageManager.images)
    {
        if (image->hasPosition && (!showSelection || image->selected))
        {
            layout(image);
        }
    }

    lastViewPosition = camera.view.getCenter().y * factor * factor;
    camera.teleport(lastViewPosition);
}

void
MultiImageView::layout(std::unique_ptr<Image>& image)
{
    image->sprite.setOrigin(0, 0);
    image->scaleTo(targetImageWidth);
    auto imageHeight = image->sprite.getGlobalBounds().height;

    for (auto i = columnIndex + 1; i <= numberOfColumns + columnIndex; i++)
    {
        auto index = i % numberOfColumns;
        auto pivot = columnOffsets[columnIndex];

        if (i >= numberOfColumns)
        {
            pivot += imageHeight;
        }

        if (columnOffsets[index] < pivot)
        {
            columnIndex = index;
            break;
        }
    }

    image->setPosition(sf::Vector2f(targetImageWidth * columnIndex, columnOffsets[columnIndex]));
    columnOffsets[columnIndex] += imageHeight;

    if (image->position.y + imageHeight > camera.bottom)
    {
        camera.bottom = image->position.y + imageHeight;
    }
}

    void
MultiImageView::draw()
{
    for (auto& image : imageManager.images)
    {
        if (showSelection && !image->selected)
        {
            continue;
        }

        if (!image->hasPosition)
        {
            layout(image);
        }

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
MultiImageView::resize(int width, int height)
{
    int newTargetImageWidth = window.getSize().x / numberOfColumns;
    float factor = (float) newTargetImageWidth / targetImageWidth;
    targetImageWidth = newTargetImageWidth;

    lastViewPosition = camera.view.getCenter().y * factor;
    camera.view.setSize(width, height);
    camera.view.setCenter(width / 2, lastViewPosition);
    camera.teleport(lastViewPosition);

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

std::pair<int, int>
MultiImageView::calcProgress()
{
    int index = std::distance(imageManager.images.cbegin(),
            std::find(imageManager.images.cbegin(), imageManager.images.cend(), *lastVisibleImage)) + 1;

    int max = !showSelection ? imageManager.numberOfFiles() :
        std::ranges::count_if(imageManager.images, 
                [](const std::unique_ptr<Image>& image) { return image->selected; });

    return { index, max };
}
