#include "ScrollView.h"
#include <iostream>
#include <set>

ScrollView::ScrollView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
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
                scrollToPosition(window.getView().getCenter().y + imageSize());
            else if (event.mouseWheelScroll.delta > 0)
                scrollToPosition(window.getView().getCenter().y - imageSize());
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                    scrollToPosition(window.getView().getCenter().y - imageSize());
                    break;
                case sf::Keyboard::Down:
                    scrollToPosition(window.getView().getCenter().y + imageSize());
                    break;
                case sf::Keyboard::PageUp:
                    scrollToPosition(window.getView().getCenter().y - window.getView().getSize().y);
                    break;
                case sf::Keyboard::PageDown:
                    scrollToPosition(window.getView().getCenter().y + window.getView().getSize().y);
                    break;
                case sf::Keyboard::Num1:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 1.f, 2));
                    numberOfColumns = 1;
                    break;
                case sf::Keyboard::Num2:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 2.f, 2));
                    numberOfColumns = 2;
                    break;
                case sf::Keyboard::Num3:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 3.f, 2));
                    numberOfColumns = 3;
                    break;
                case sf::Keyboard::Num4:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 4.f, 2));
                    numberOfColumns = 4;
                    break;
                case sf::Keyboard::Num5:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 5.f, 2));
                    numberOfColumns = 5;
                    break;
                case sf::Keyboard::Num6:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 6.f, 2));
                    numberOfColumns = 6;
                    break;
                case sf::Keyboard::Num7:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 7.f, 2));
                    numberOfColumns = 7;
                    break;
                case sf::Keyboard::Num8:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 8.f, 2));
                    numberOfColumns = 8;
                    break;
                case sf::Keyboard::Num9:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 9.f, 2));
                    numberOfColumns = 9;
                    break;
                case sf::Keyboard::Num0:
                    scrollToPosition(window.getView().getCenter().y * std::pow(numberOfColumns / 10.f, 2));
                    numberOfColumns = 10;
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
ScrollView::draw()
{
    int windowTop = window.getView().getCenter().y - window.getView().getSize().y / 2;
    windowTop = (windowTop < 0) ? 0 : windowTop;

    int size = imageSize();
    int firstVisibleRow = windowTop / size;
    int firstImage = firstVisibleRow * numberOfColumns;

    auto iter = folder.cbegin();
    std::advance(iter, firstImage);

    int windowBot = window.getView().getCenter().y + window.getView().getSize().y / 2;
    int lastVisibleRow = std::ceil(1.f * windowBot / size);
    int lastImage = lastVisibleRow * numberOfColumns;

    auto end = folder.cbegin();
    std::advance(end, std::min(lastImage, folder.size()));

    std::set<std::string> unusedImages;
    for (auto const& entry: imageCache)
        unusedImages.insert(entry.first);

    int heightOffset = firstVisibleRow * size;
    int column = 0;

    while (iter != end)
    {
        Image* image = getImage(*iter);
        unusedImages.erase(*iter);
        if (image->valid)
        {
            image->update();
            float factor = size / image->sprite.getLocalBounds().width;
            image->sprite.setScale(factor, factor);
            image->sprite.setPosition(size * column, heightOffset);
            window.draw(image->sprite);
        }

        iter++;
        column++;
        if (column == numberOfColumns)
        {
            heightOffset += size;
            column = 0;
        }
    }

    for (auto const& path: unusedImages) {
        delete imageCache[path];
        imageCache.erase(path);
    }
}

Image*
ScrollView::getImage(const std::string& path)
{
    if (imageCache.count(path) == 0)
    {
        Image* image = new Image(path, true);
        imageCache.insert(std::pair<std::string, Image*>(path, image));
    }
    return imageCache.find(path)->second;
}

void
ScrollView::resizeEvent(sf::Event::SizeEvent& size)
{
    sf::View view = window.getView();
    float y = view.getCenter().y * size.width / view.getSize().x;
    view.setSize(size.width, size.height);
    view.setCenter(size.width / 2, y);
    window.setView(view);
}

void
ScrollView::selectImage()
{
    auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto const& entry: imageCache)
        if (entry.second->sprite.getGlobalBounds().contains(mouse.x, mouse.y))
            folder.currentItem = std::find(folder.cbegin(), folder.cend(), entry.first);
}

void
ScrollView::scrollToCurrentImage()
{
    int index = std::distance(folder.cbegin(), folder.currentItem);
    scrollToPosition(index / numberOfColumns * imageSize() + imageSize() / 2);
}

void
ScrollView::scrollToPosition(float y)
{
    if (y < window.getSize().y / 2)
        y = window.getSize().y / 2;
    else if (y > std::ceil(1.f * folder.size() / numberOfColumns) * imageSize() - window.getSize().y / 2)
        y = std::ceil(1.f * folder.size() / numberOfColumns) * imageSize() - window.getSize().y / 2;
    else
        y = std::floor((y - window.getSize().y / 2) / imageSize()) * imageSize() + window.getSize().y / 2;

    auto view = window.getView();
    view.setCenter(view.getCenter().x, y);
    window.setView(view);
}

int
ScrollView::imageSize()
{
    return window.getSize().x / numberOfColumns;
}
