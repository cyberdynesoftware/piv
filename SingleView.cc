#include "SingleView.h"
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <iostream>

SingleView::SingleView(Folder& folder, sf::RenderWindow& window):
    folder(folder),
    window(window)
{
    cross.loadFromSystem(sf::Cursor::Cross);
    previousMousePosition = sf::Mouse::getPosition();
}

bool
SingleView::instanceOf(const SubType& subType)
{
    return subType == SubType::SingleView;
}

void
SingleView::init()
{
    loadImage(*folder.currentItem);
    fitToScreen(sprite);
}

bool
SingleView::loadImage(const std::string& path)
{
    OIIO::ustring upath = OIIO::ustring(path.c_str());

    OIIO::ImageSpec spec;
    bool ok = folder.imageCache->get_imagespec(upath, spec);
    if (!ok)
    {
        std::cout << "\nERROR: " << path << std::endl;
        std::cerr << folder.imageCache->geterror() << std::endl;
        return false;
    }

    OIIO::ImageBuf buffer(upath);

    if (spec.nchannels == 3)
        buffer = OIIO::ImageBufAlgo::channels(buffer, 4,
                /* channelorder */ { 0, 1, 2, -1 /*use a float value*/ },
                /* channelvalues */ { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 },
                /* channelnames */ { "", "", "", "A" });
    else if (spec.nchannels != 4)
        std::cerr << "Error: " << path << ": nchannels = " << spec.nchannels << std::endl;

    sf::Uint8 pixels[spec.width * spec.height * 4];
    ok = buffer.get_pixels(buffer.roi(), OIIO::TypeDesc::UINT8, pixels);
    if (!ok || buffer.has_error())
    {
        std::cout << "\nERROR: " << path << std::endl;
        std::cerr << buffer.geterror() << std::endl;
        return false;
    }

    texture.create(spec.width, spec.height);
    texture.update(pixels, spec.width, spec.height, 0, 0);
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
                    loadImage(*folder.currentItem);
                    fitToScreen(sprite);
                    break;
                case sf::Keyboard::Backspace:
                    if (folder.currentItem != folder.cbegin())
                        folder.currentItem--;
                    loadImage(*folder.currentItem);
                    fitToScreen(sprite);
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
    //fitToScreen(sprite);
    window.draw(sprite);
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
