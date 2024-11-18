#include "ImageView.h"

bool
ImageView::showInfo = false;

void
ImageView::setupInfoBox(const std::unique_ptr<Image>& image)
{
    info.setMessage(image->info)
        .setColor(sf::Color::White)
        .setSize(15)
        .setWidth(image->sprite.getGlobalBounds().width)
        .setPosition(image->sprite.getGlobalBounds().getPosition());
}
