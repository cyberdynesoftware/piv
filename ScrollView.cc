#include "ScrollView.h"
#include <iostream>
#include <set>

ScrollView::ScrollView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
    firstItem = folder.currentItem;
    lastItem = folder.currentItem;
    initImages();
}

bool
ScrollView::instanceOf(const SubType& subType)
{
    return subType == SubType::ScrollView;
}

void
ScrollView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.delta < 0)
                scrollDown(1);
            else if (event.mouseWheelScroll.delta > 0)
                scrollUp(1);
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                    scrollUp(1);
                    break;
                case sf::Keyboard::Down:
                    scrollDown(1);
                    break;
                case sf::Keyboard::PageUp:
                    scrollUp(1);
                    break;
                case sf::Keyboard::PageDown:
                    scrollDown(1);
                    break;
                case sf::Keyboard::Num1:
                    numberOfColumns = 1;
                    initImages();
                    break;
                case sf::Keyboard::Num2:
                    numberOfColumns = 2;
                    initImages();
                    break;
                case sf::Keyboard::Num3:
                    numberOfColumns = 3;
                    initImages();
                    break;
                case sf::Keyboard::Num4:
                    numberOfColumns = 4;
                    initImages();
                    break;
                case sf::Keyboard::Num5:
                    numberOfColumns = 5;
                    initImages();
                    break;
                case sf::Keyboard::Num6:
                    numberOfColumns = 6;
                    initImages();
                    break;
                case sf::Keyboard::Num7:
                    numberOfColumns = 7;
                    initImages();
                    break;
                case sf::Keyboard::Num8:
                    numberOfColumns = 8;
                    initImages();
                    break;
                case sf::Keyboard::Num9:
                    numberOfColumns = 9;
                    initImages();
                    break;
                case sf::Keyboard::Num0:
                    numberOfColumns = 10;
                    initImages();
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
ScrollView::initImages()
{
    int index = std::distance(folder.cbegin(), firstItem);
    int row =  index / numberOfColumns;
    int firstImage = row * numberOfColumns;
    int indent = index - firstImage;

    for (int i = 0; i < indent; i++)
    {
        Image* image = new Image(*--firstItem, true);
        images.push_front(image);
    }

    int rows = std::ceil(1.f * window.getSize().y / imageSize());
    int imageCount = rows * numberOfColumns;
    int delta = imageCount - images.size();

    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
        {
            Image* image = new Image(*lastItem++, true);
            images.push_back(image);

            if (lastItem == folder.cend()) break;
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
    std::cout << "Images: " << images.size() << std::endl;
}

void
ScrollView::scrollDown(int rows)
{
    if (lastItem == folder.cend())
    {
        // move view to bottom
    }
    else
    {
        for (int i = 0; i < numberOfColumns * rows; i++)
        {
            delete images.front();
            images.pop_front();
            firstItem++;
        }

        for (int i = 0; i < numberOfColumns * rows; i++)
        {
            Image* image = new Image(*lastItem++, true);
            images.push_back(image);

            if (lastItem == folder.cend()) break;
        }
    }
}

void
ScrollView::scrollUp(int rows)
{
    if (firstItem == folder.cbegin()) return;

    int removeCount = numberOfColumns * rows;
    if (lastItem == folder.cend())
        removeCount = folder.size() % numberOfColumns + numberOfColumns * (rows - 1);

    for (int i = 0; i < removeCount; i++)
    {
        delete images.back();
        images.pop_back();
        lastItem--;
    }

    for (int i = 0; i < numberOfColumns * rows; i++)
    {
        Image* image = new Image(*--firstItem, true);
        images.push_front(image);

        if (firstItem == folder.cbegin()) break;
    }
}

void
ScrollView::draw()
{
    int heightOffset = 0;
    int column = 0;

    for (auto image : images)
    {
        if (image->valid)
        {
            image->update();
            float factor = imageSize() / image->sprite.getLocalBounds().width;
            image->sprite.setScale(factor, factor);
            image->sprite.setPosition(imageSize() * column, heightOffset);
            window.draw(image->sprite);
        }

        column++;
        if (column == numberOfColumns)
        {
            heightOffset += imageSize();
            column = 0;
        }
    }
}

void
ScrollView::resizeEvent(sf::Event::SizeEvent& size)
{
    initImages();
}

void
ScrollView::selectImage()
{
    auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto const& image: images)
        if (image->sprite.getGlobalBounds().contains(mouse.x, mouse.y))
            folder.currentItem = std::find(folder.cbegin(), folder.cend(), image->path);
}

void
ScrollView::scrollToCurrentImage()
{
}

int
ScrollView::imageSize()
{
    return window.getSize().x / numberOfColumns;
}
