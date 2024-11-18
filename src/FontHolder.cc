#include "FontHolder.h"
#include "font.h"

sf::Font FontHolder::font;

void
FontHolder::init()
{
    font.loadFromMemory(font_ttf, font_ttf_len);
}
