#include "SingleView.h"

SingleView::SingleView(ImageCache& imageCache, sf::RenderWindow& window):
    imageCache(imageCache),
    window(window)
{
    arrow.loadFromSystem(sf::Cursor::Arrow);
    cross.loadFromSystem(sf::Cursor::Cross);
    previousMousePosition = sf::Mouse::getPosition();
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
                    imageIter++;
                    fitToScreen((**imageIter).getSprite());
                    break;
                case sf::Keyboard::Backspace:
                    imageIter--;
                    fitToScreen((**imageIter).getSprite());
                    break;
                case sf::Keyboard::R:
                    //imageCache.random();
                    fitToScreen((**imageIter).getSprite());
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
            zoom((**imageIter).getSprite(), event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    window.setMouseCursor(cross);
                    break;
                case sf::Mouse::Button::Right:
                    fitToScreen((**imageIter).getSprite());
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
                (**imageIter).getSprite().move(delta.x, delta.y);
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
    window.draw((**imageIter).getSprite());
}

void
SingleView::fitToScreen(sf::Sprite& sprite)
{
    const sf::Vector2u& size = sprite.getTexture()->getSize();

    float xScale = (float)window.getSize().x / size.x;
    float yScale = (float)window.getSize().y / size.y;

    if (xScale < yScale)
        sprite.setScale(xScale, xScale);
    else
        sprite.setScale(yScale, yScale);

    sprite.setOrigin(size.x / 2, size.y / 2);
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

void
SingleView::select(ImageCache::ImageIter iter)
{
    imageIter = iter;
    fitToScreen((**imageIter).getSprite());
}
