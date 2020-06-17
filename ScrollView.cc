#include "ScrollView.h"

ScrollView::ScrollView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    layout();
}

void
ScrollView::layout()
{
    sprite = &imageCache.next();
    //window.draw(imageCache.next());
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
    window.draw(*sprite);
}
