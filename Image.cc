#include "Image.h"

void
Image::load(const std::string& path)
{
    OIIO::ustring upath = OIIO::ustring(path.c_str());
    OIIO::ImageBuf buffer(upath);

    if (buffer.spec().get_int_attribute("oiio:Movie", 0))
    {
        int fps[2];
        OIIO::TypeDesc type = buffer.spec().getattributetype("FramesPerSecond");
        buffer.spec().getattribute("FramesPerSecond", type, &fps);
        std::cout << path << ": " << buffer.nsubimages() <<  ", " << type <<  ", " << fps[0] << ":" << fps[1] << "; " << buffer.spec().get_float_attribute("FramesPerSecond") << std::endl;
    }
    if (buffer.nsubimages() == 0)
    {
        errormsg = "Not an image: " + path;
        valid = false;
        return;
    }

    if (buffer.nchannels() == 3)
        buffer = OIIO::ImageBufAlgo::channels(buffer, 4,
                /* channelorder */ { 0, 1, 2, -1 /*use a float value*/ },
                /* channelvalues */ { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 },
                /* channelnames */ { "", "", "", "A" });

    sf::Uint8 *pixels = new sf::Uint8[buffer.roi().width() * buffer.roi().height() * 4];
    bool ok = buffer.get_pixels(buffer.roi(), OIIO::TypeDesc::UINT8, pixels);
    if (!ok || buffer.has_error())
    {
        errormsg = "Error loading image: " + path;
        valid = false;
        return;
    }

    texture.create(buffer.roi().width(), buffer.roi().height());
    texture.update(pixels);
    delete[] pixels;
    texture.setSmooth(true);
    sprite.setTexture(texture, true);
    valid = true;
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
