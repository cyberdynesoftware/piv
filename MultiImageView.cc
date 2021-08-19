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
                    scrollUp();
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::J:
                    scrollDown();
                    break;
                case sf::Keyboard::PageDown:
                    if (lastItem != folder.cend())
                        scrollTo(++lastItem);
                    break;
                case sf::Keyboard::Home:
                    scrollTo(folder.cbegin());
                    break;
                case sf::Keyboard::End:
                    scrollTo(folder.cend());
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
MultiImageView::initImages()
{
    int index = std::distance(folder.cbegin(), firstItem);
    int row =  index / numberOfColumns;
    int firstImage = row * numberOfColumns;
    int indent = index - firstImage;

    for (int i = 0; i < indent; i++)
    {
        Image* image = new Image(*--firstItem);
        images.push_front(image);
    }

    int rows = std::ceil(1.f * window.getSize().y / targetImageWidth);
    int imageCount = rows * numberOfColumns;
    int delta = imageCount - images.size();

    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
        {
            if (lastItem == folder.cend()) break;

            Image* image = new Image(*lastItem++);
            images.push_back(image);
        }

        delta = imageCount - images.size();
        rows = delta / numberOfColumns;
        int remaining = rows * numberOfColumns;

        for (int i = 0; i < remaining; i++)
        {
            if (firstItem == folder.cbegin()) break;

            Image* image = new Image(*--firstItem);
            images.push_front(image);
        }
    }
    else 
    {
        for (int i = 0; i < -delta; i++)
        {
            delete images.back();
            images.pop_back();
            lastItem--;
        }
    }

    for (auto image : images)
        image->square(targetImageWidth);
}

void
MultiImageView::scroll(int delta)
{
    sf::View view = window.getView();
    view.move(0, delta * -10);
    window.setView(view);
}

void
MultiImageView::scrollDown()
{
    if (lastItem == folder.cend())
    {
        int rows = std::ceil(1.f * window.getSize().y / targetImageWidth);
        heightOffset = window.getSize().y - rows * targetImageWidth;
    }
    else
    {
        for (int i = 0; i < numberOfColumns; i++)
        {
            delete images.front();
            images.pop_front();
            firstItem++;
        }

        for (int i = 0; i < numberOfColumns; i++)
        {
            if (lastItem == folder.cend()) break;

            Image* image = new Image(*lastItem++);
            image->square(targetImageWidth);
            images.push_back(image);
        }
    }
}

void
MultiImageView::scrollUp()
{
    heightOffset = 0;
    if (firstItem == folder.cbegin()) return;

    int removeCount = (lastItem == folder.cend()) ? folder.size() % numberOfColumns : numberOfColumns;

    for (int i = 0; i < removeCount; i++)
    {
        delete images.back();
        images.pop_back();
        lastItem--;
    }

    for (int i = 0; i < numberOfColumns; i++)
    {
        if (firstItem == folder.cbegin()) break;

        Image* image = new Image(*--firstItem);
        image->square(targetImageWidth);
        images.push_front(image);
    }
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
MultiImageView::resizeEvent()
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
    //view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(view.getCenter().x, viewPosition);
    window.setView(view);
}

bool
MultiImageView::selectImage()
{
    auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto const& image: images)
        if (image->sprite.getGlobalBounds().contains(mouse.x, mouse.y))
        {
            folder.currentItem = std::find(folder.cbegin(), folder.cend(), image->path);
            return true;
        }
    return false;
}

void
MultiImageView::scrollToCurrentImage()
{
    if (folder.currentItem < firstItem || folder.currentItem >= lastItem)
        scrollTo(folder.currentItem);
}

void
MultiImageView::scrollTo(const Folder::FolderIter& item)
{
    for (Image* image: images)
        delete image;

    images.clear();

    firstItem = item;
    lastItem = item;
    initImages();
}

float
MultiImageView::viewBottom()
{
    return window.getView().getCenter().y + window.getView().getSize().y;
}
