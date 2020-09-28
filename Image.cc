#include "Image.h"
#include <OpenImageIO/imagebufalgo.h>
#include <iostream>

Image::Image()
{
    buffer = NULL;
}

Image::~Image()
{
    delete buffer;
}

void
Image::init(const std::string& path)
{
    //delete buffer;
    buffer = new OIIO::ImageBuf(path);

    if (buffer->nsubimages() == 0)
    {
        errormsg = "Not an image: " + path;
        valid = false;
    }
    else
    {
        load();
    }
}

void
Image::load()
{
    if (buffer->nchannels() == 3)
        addAlphaChannel();

    sf::Uint8 *pixels = new sf::Uint8[buffer->roi().width() * buffer->roi().height() * 4];
    bool ok = buffer->get_pixels(buffer->roi(), OIIO::TypeDesc::UINT8, pixels);
    if (!ok || buffer->has_error())
    {
        errormsg = "Error loading image: ";// + path;
        valid = false;
        return;
    }

    texture.create(buffer->roi().width(), buffer->roi().height());
    texture.update(pixels);
    texture.setSmooth(true);
    sprite.setTexture(texture, true);

    delete[] pixels;
    valid = true;
    clock.restart();
}

void
Image::addAlphaChannel()
{
    OIIO::ImageBuf *newbuf = new OIIO::ImageBuf(OIIO::ImageBufAlgo::channels(*buffer, 4,
                /* channelorder */ { 0, 1, 2, -1 /*use a float value*/ },
                /* channelvalues */ { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 },
                /* channelnames */ { "", "", "", "A" }));
    delete buffer;
    buffer = newbuf;
}

void
Image::fitTo(const sf::Vector2u& window)
{
    if (valid)
    {
        const sf::Vector2u& size = sprite.getTexture()->getSize();

        float xScale = (float)window.x / size.x;
        float yScale = (float)window.y / size.y;
        float scale = (xScale < yScale) ? xScale : yScale;
        sprite.setScale(scale, scale);

        sprite.setOrigin(size.x / 2, size.y / 2);
        sprite.setPosition(window.x / 2, window.y / 2);
    }
}

void
Image::update()
{
    if (buffer->nsubimages() == 1)
        return;

    float fps = buffer->spec().get_float_attribute("FramesPerSecond");
    if (clock.getElapsedTime() > sf::seconds(1 / fps))
    {
        int index = buffer->subimage() + 1;
        if (index == buffer->nsubimages())
            index = 0;

        buffer->read(index);
        load();

        clock.restart();
    }
}

void
Image::square()
{
    if (valid)
    {
        auto size = sprite.getTexture()->getSize();
        auto offset = std::abs((int)size.x - (int)size.y) / 2;

        if (size.x < size.y)
            sprite.setTextureRect(sf::IntRect(0, offset, size.x, size.x));
        else
            sprite.setTextureRect(sf::IntRect(offset, 0, size.y, size.y));
    }
}
