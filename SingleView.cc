#include "SingleView.h"
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <iostream>

SingleView::SingleView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
    arrow.loadFromSystem(sf::Cursor::Arrow);
    cross.loadFromSystem(sf::Cursor::Cross);
    font.loadFromFile("font.ttf");
    text.setFont(font);
    text.setCharacterSize(15);
    text.setPosition(10, 10);
    previousMousePosition = sf::Mouse::getPosition();
}

bool
SingleView::instanceOf(const SubType& subType)
{
    return subType == SubType::SingleView;
}

void
SingleView::initImage()
{
    window.setTitle(*folder.currentItem + " - piv");

    imageValid = loadImage(*folder.currentItem);
    if (imageValid)
        fitToScreen(sprite);
}

bool
SingleView::loadImage(const std::string& path)
{
    OIIO::ustring upath = OIIO::ustring(path.c_str());
    OIIO::ImageBuf buffer(upath);

    if (buffer.spec().get_int_attribute("oiio:Movie", 0))
    {
        int fps[2];
        OIIO::TypeDesc type = buffer.spec().getattributetype("FramesPerSecond");
        buffer.spec().getattribute("FramesPerSecond", type, &fps);
        std::cout << path << ": " << buffer.nsubimages() <<  ", " << type <<  ", " << fps[0] << ":" << fps[1] << std::endl;
    }
    if (buffer.nsubimages() == 0)
    {
        text.setString("Not an image: " + path);
        return false;
    }

    if (buffer.nchannels() == 3)
        buffer = OIIO::ImageBufAlgo::channels(buffer, 4,
                /* channelorder */ { 0, 1, 2, -1 /*use a float value*/ },
                /* channelvalues */ { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 },
                /* channelnames */ { "", "", "", "A" });
    else if (buffer.nchannels() != 4)
        std::cerr << "Error: " << path << ": nchannels = " << buffer.nchannels() << std::endl;

    sf::Uint8 pixels[buffer.roi().width() * buffer.roi().height() * 4];
    bool ok = buffer.get_pixels(buffer.roi(), OIIO::TypeDesc::UINT8, pixels);
    if (!ok || buffer.has_error())
    {
        text.setString("Error loading image: " + path);
        return false;
    }

    texture.create(buffer.roi().width(), buffer.roi().height());
    texture.update(pixels, buffer.roi().width(), buffer.roi().height(), 0, 0);
    texture.setSmooth(true);
    sprite.setTexture(texture, true);
    return true;
}

void
SingleView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Space:
                    if (++folder.currentItem == folder.cend())
                        folder.currentItem--;
                    initImage();
                    break;
                case sf::Keyboard::Backspace:
                    if (folder.currentItem != folder.cbegin())
                        folder.currentItem--;
                    initImage();
                    break;
                case sf::Keyboard::R:
                    //imageCache.random();
                    //fitToScreen((**imageCache.currentImage).getSprite());
                    break;
                case sf::Keyboard::A:
                    //folder.select();
                    break;
                case sf::Keyboard::D:
                    //folder.trash();
                    break;
                case sf::Keyboard::O:
                    sprite.setScale(1.0, 1.0);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseWheelScrolled:
            zoom(sprite, event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    fitToScreen(sprite);
                    break;
                default:
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(arrow);
                    break;

                default:
                    break;
            }
            break;

        case sf::Event::MouseMoved:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i delta(sf::Mouse::getPosition() - previousMousePosition);
                sprite.move(delta.x, delta.y);
            }

            previousMousePosition = sf::Mouse::getPosition();
            break;

        default:
            break;
    }
}

void
SingleView::draw()
{
    if (imageValid)
        window.draw(sprite);
    else
        window.draw(text);
}

void
SingleView::fullscreenToggle()
{
    window.setTitle(*folder.currentItem + " - piv");

    if (imageValid)
        fitToScreen(sprite);
}

void
SingleView::fitToScreen(sf::Sprite& sprite)
{
    const sf::Vector2u& size = sprite.getTexture()->getSize();

    float xScale = (float)window.getSize().x / size.x;
    float yScale = (float)window.getSize().y / size.y;
    float scale = (xScale < yScale) ? xScale : yScale;

    sprite.setOrigin(size.x / 2, size.y / 2);
    sprite.setScale(scale, scale);
    sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}

void
SingleView::zoom(sf::Sprite& sprite, float delta)
{
    if (delta < 0)
    {
        sprite.scale(0.95f, 0.95f);
    }
    else if (delta > 0)
    {
        sprite.setOrigin(mousePositionInSprite(sprite) / sprite.getScale().x);
        sprite.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        sprite.scale(1.05f, 1.05f);
    }
}

sf::Vector2f
SingleView::mousePositionInSprite(sf::Sprite& sprite)
{
    return sf::Vector2f(sf::Mouse::getPosition().x - sprite.getGlobalBounds().left,
            sf::Mouse::getPosition().y - sprite.getGlobalBounds().top);
}
