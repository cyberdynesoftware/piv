#include "ScrollView.h"

ScrollView::ScrollView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    imageCache.loadImages(5);
}

void
ScrollView::handle(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseWheelScrolled:
            //zoom(imageCache.getSprite(), event.mouseWheelScroll.delta);
            break;

        default:
            break;
    }
}

void
ScrollView::draw()
{
    for (ImageCache::ImageIter iter = imageCache.cbegin(); iter != imageCache.cend(); iter++)
    {
        window.draw((**iter).getSprite());
    }
}
