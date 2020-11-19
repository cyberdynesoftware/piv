#include "MultiImageView.h"
#include <iostream>
#include <set>
#include <cmath>

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
                scrollDown();
            else if (event.mouseWheelScroll.delta > 0)
                scrollUp();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                    scrollUp();
                    break;
                case sf::Keyboard::Down:
                    scrollDown();
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
ScrollView::initImages()
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

    int rows = std::ceil(1.f * window.getSize().y / imageSize());
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
        image->square(imageSize());
}

void
ScrollView::scrollDown()
{
    if (lastItem == folder.cend())
    {
        int rows = std::ceil(1.f * window.getSize().y / imageSize());
        heightOffset = window.getSize().y - rows * imageSize();
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
            image->square(imageSize());
            images.push_back(image);
        }
    }
}

void
ScrollView::scrollUp()
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
        image->square(imageSize());
        images.push_front(image);
    }
}

void
ScrollView::draw()
{
    int offset = heightOffset;
    int column = 0;
    int size = imageSize();

    for (auto image : images)
    {
        if (image->ready)
        {
            image->update();
            image->sprite.setPosition(size * column, offset);
            window.draw(image->sprite);
        }

        if (showInfo)
        {
            sf::RectangleShape background(sf::Vector2f(size, 20));
            background.setFillColor(sf::Color(0, 0, 0, 64));
            background.setPosition(size * column, offset + size - 20);
            window.draw(background);

            sf::Text info;
            info.setFont(font);
            info.setFillColor(sf::Color::White);
            info.setCharacterSize(16);
            info.setPosition(size * column, offset + size - 20);
            info.setString(Folder::filename(image->path));
            window.draw(info);
        }

        column++;
        if (column == numberOfColumns)
        {
            offset += size;
            column = 0;
        }
    }
}

void
ScrollView::resizeEvent()
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
    if (folder.currentItem < firstItem || folder.currentItem >= lastItem)
    {
        for (Image* image: images)
            delete image;

        images.clear();

        firstItem = folder.currentItem;
        lastItem = folder.currentItem;
        initImages();
    }
}

int
ScrollView::imageSize()
{
    return window.getSize().x / numberOfColumns;
}
