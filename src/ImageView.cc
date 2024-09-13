#include "ImageView.h"

bool ImageView::showInfo = false;

void
ImageView::setupInfoBox(const std::unique_ptr<Image>& image)
{
    info.setTextAndWidth(image->info, image->sprite.getGlobalBounds().width);
    info.setPosition(image->sprite.getGlobalBounds().getPosition());
}
