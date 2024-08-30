#include "MultiImageView.h"
#include "Help.h"
#include <iostream>
#include <set>
#include <cmath>

MultiImageView::MultiImageView(Folder& folder, sf::RenderWindow& window, ImageManager& imageManager):
    folder(folder),
    window(window),
    imageManager(imageManager),
    gui(window),
    columnOffsets(numberOfColumns, 0)
{
    folderIter = folder.cbegin();
    targetImageWidth = window.getSize().x / numberOfColumns;
    viewPosition = lastViewPosition = window.getView().getCenter().y;
    viewHeight = window.getView().getSize().y;
    highlightBackground.setFillColor(sf::Color(0, 0, 0, 224));
    highlightBackground.setSize(window.getView().getSize());
    highlightBackground.setOrigin(window.getView().getSize() / 2.f);
    imageManager.loadImages(numberOfColumns);
    previousMousePosition = sf::Mouse::getPosition();
    view = window.getView();

    if (folder.selectedFolderExistsNotEmpty())
    {
        gui.showSelectedFolderWarning = true;
    }

    gui.helpMsg(generateHelpText());
}
/*
void
MultiImageView::loadImageRow()
{
    for (int i = 0; i < numberOfColumns; i++)
    {
        if (folderIter == folder.cend()) break;
        images.push_back(new Image(*folderIter++));
    }
}
*/
void
MultiImageView::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            if (elevatedImage == NULL)
                setViewPosition(viewPosition - event.mouseWheelScroll.delta * viewHeight / 100);
            else
                zoom(event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::XButton1:
                    if (elevatedImage == NULL)
                        scrollState = DOWN;
                    else
                        nextImage();
                    break;
                case sf::Mouse::Button::XButton2:
                    if (elevatedImage == NULL)
                        scrollState = UP;
                    else
                        previousImage();
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
                case sf::Mouse::Button::Middle:
                    selectImage();
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseMoved:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && elevatedImage != NULL)
            {
                sf::Vector2i delta(sf::Mouse::getPosition() - previousMousePosition);
                elevatedImage->sprite.move(delta.x, delta.y);
            }

            previousMousePosition = sf::Mouse::getPosition();
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
                    gui.showInfo = (gui.showInfo) ? false : true;
                    break;
                case sf::Keyboard::M:
                    unpickImage();
                    break;
                case sf::Keyboard::A:
                    selectImage();
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

                        gui.helpMsg(generateHelpText());
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
                    break;
                case sf::Keyboard::Space:
                    if (elevatedImage == NULL)
                        scrollState = (scrollState == NONE) ? AUTO_SCROLL : NONE;
                    else
                        nextImage();
                    break;
                case sf::Keyboard::Backspace:
                    if (elevatedImage != NULL)
                        previousImage();
                    break;
                case sf::Keyboard::Home:
                case sf::Keyboard::G:
                    setViewPosition(window.getView().getSize().y / 2);
                    scrollSpeed = 0;
                    break;
                case sf::Keyboard::H:
                    gui.showHelp = true;
                    break;
                case sf::Keyboard::O:
                    if (elevatedImage != NULL)
                    {
                        if (elevatedImage->sprite.getScale().x == 1.f)
                            elevatedImage->fitTo(window.getView());
                        else
                            elevatedImage->sprite.setScale(1.f, 1.f);
                    }
                    break;
                case sf::Keyboard::Y:
                    if (showSelection && elevatedImage == NULL)
                        for (auto image : images)
                            if (image->selected)
                                folder.copyToSelection(image->path);
                    break;
                case sf::Keyboard::X:
                    if (showSelection && elevatedImage == NULL)
                    {
                        std::deque<Image*> temp;
                        for (auto image : images)
                            if (image->selected)
                            {
                                folder.moveToSelection(image->path);
                                delete image;
                            }
                            else
                                temp.push_back(image);

                        folder.scan();
                        images = temp;
                        folderIter = std::find(folder.cbegin(), folder.cend(), images.back()->path);
                        folderIter++;
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
                case sf::Keyboard::H:
                    gui.showHelp = false;
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
    //sf::View view = window.getView();
    view.setCenter(view.getCenter().x, centerY);

    if (view.getCenter().y - view.getSize().y / 2 < 0 || bottom < view.getSize().y)
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    else if ((folderIter == folder.cend() || showSelection) && view.getCenter().y + view.getSize().y / 2 > bottom)
        view.setCenter(view.getCenter().x, bottom - view.getSize().y / 2);

    //window.setView(view);
    viewPosition = view.getCenter().y;
}

void
MultiImageView::pickImage()
{
    if (elevatedImage == NULL)
    {
        elevatedImage = findImageUnderMouse();

        if (elevatedImage != NULL)
            elevatedImage->fitTo(view);

        imageIter = std::find(images.begin(), images.end(), elevatedImage);

        gui.helpMsg(generateHelpText());
    }
}

Image*
MultiImageView::findImageUnderMouse()
{
    window.setView(view);
    auto mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto image : images)
    {
        if (showSelection && !image->selected) continue;
        if (image->sprite.getGlobalBounds().contains(mouseCoords.x, mouseCoords.y))
            return image;
    }

    return NULL;
}

void
MultiImageView::selectImage()
{
    if (elevatedImage == NULL)
    {
        Image* image = findImageUnderMouse();
        image->selected = !image->selected;
    }
    else
    {
        elevatedImage->selected = !elevatedImage->selected;
        nextImage();
    }
}

void
MultiImageView::nextImage()
{
    while (imageIter != images.end() && ++imageIter != images.end())
        if ((*imageIter)->valid)
        {
            unpickImage();
            elevatedImage = *imageIter;
            elevatedImage->fitTo(view);
            break;
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

        scrollSpeed = 0;

        gui.helpMsg(generateHelpText());
    }
}

void
MultiImageView::previousImage()
{
    while (imageIter != images.begin())
        if ((*(--imageIter))->valid)
        {
            unpickImage();
            elevatedImage = *imageIter;
            elevatedImage->fitTo(view);
            break;
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
}

void
MultiImageView::layout(Image* image)
{
    const sf::Vector2u& imageSize = image->sprite.getTexture()->getSize();
    float scale = (float)targetImageWidth / imageSize.x;
    int imageHeight = imageSize.y * scale;
    image->sprite.setScale(scale, scale);
    image->sprite.setOrigin(0, 0);

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
        if (image == elevatedImage) continue;
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

    if (elevatedImage != NULL)
    {
        highlightBackground.setPosition(view.getCenter());
        window.draw(highlightBackground);
        elevatedImage->update();
        window.draw(elevatedImage->sprite);
        gui.drawInfoBox(elevatedImage);
        if (!showSelection  && elevatedImage->selected) gui.drawSelectedIcon(elevatedImage);
    }

    if (viewPosition + viewHeight / 2 > columnOffsets[minColumnIndex()] && !showSelection)
        imageManager.loadImages(numberOfColumns);

    gui.update();
    window.draw(gui);

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
    if (elevatedImage == NULL)
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
}

float
MultiImageView::calcProgress()
{
    int index = std::find(folder.cbegin(), folder.cend(), lastVisibleImage->path) - folder.cbegin();
    int max = 0;

    if (showSelection)
    {
        for (auto image : images)
            if (image->selected)
                max++;
    }
    else
    {
        max = folder.size();
    }

    auto progress =  (float) index / max;
    auto msg = std::to_string(index) + " / " + std::to_string(max);
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

    for (auto image : images)
    {
        if (image->hasPosition)
        {
            image->setPosition(image->position * factor);
            image->sprite.scale(factor, factor);
            if (image == elevatedImage)
                elevatedImage->fitTo(view);
        }
    }

    for (int i = 0; i < numberOfColumns; i++)
        columnOffsets[i] *= factor;

    bottom *= factor;
}

std::string
MultiImageView::generateHelpText()
{
    std::string help = Help::general();

    if (elevatedImage != NULL)
    {
        help.append(Help::singleImage());
    }
    else
    {
        help.append(Help::allImages());
        if (showSelection)
            help.append(Help::selectedImages());
    }

    return help;
}

void
MultiImageView::zoom(float delta)
{
    if (delta < 0)
    {
        elevatedImage->sprite.scale(0.95f, 0.95f);
    }
    else if (delta > 0)
    {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f mousePositionInSprite(mousePosition.x - elevatedImage->sprite.getGlobalBounds().left,
                mousePosition.y - elevatedImage->sprite.getGlobalBounds().top);

        elevatedImage->sprite.setOrigin(mousePositionInSprite / elevatedImage->sprite.getScale().x);
        elevatedImage->sprite.setPosition(mousePosition);
        elevatedImage->sprite.scale(1.05f, 1.05f);
    }
}
