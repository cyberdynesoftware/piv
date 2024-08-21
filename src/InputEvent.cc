#include "InputEvent.h"

InputEvent::InputEvent(WindowManager& wm, MultiImageView& iv)
    :
        windowManager(wm),
        imageView(iv)
{
}

    void
InputEvent::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            windowManager.closeWindow();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Escape:
                    windowManager.closeWindow();
                    break;

                case sf::Keyboard::F:
                    windowManager.toggleFullscreen();
                    imageView.resize();
                    break;

                default:
                    imageView.handle(event);
                    break;
            }
            break;

        default:
            imageView.handle(event);
            break;
    }
}
