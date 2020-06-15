#include "ScrollView.h"

ScrollView::ScrollView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{ }

void
SingleView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            zoom(imageCache.getSprite(), event.mouseWheelScroll.delta);
            break;

        default:
            break;
    }
}

void
SingleView::draw()
{
    window.draw(imageCache.getSprite());
}
