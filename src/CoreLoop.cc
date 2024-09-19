#include "CoreLoop.h"
#include "icon.h"

CoreLoop::CoreLoop(char* path)
    : 
        window(sf::VideoMode(1280, 720), "piv", sf::Style::Default),
        folder(path),
        sceneManager(folder, window)
{
    window.setPosition(desktopCenterPosition());
    window.setFramerateLimit(60);

    icon.loadFromMemory(icon_png, icon_png_len);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void
CoreLoop::toggleFullscreen()
{
    if (fullscreen)
    {
        window.create(windowMode, "piv", sf::Style::Default);
        window.setPosition(desktopCenterPosition());
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        fullscreen = false;
    }
    else
    {
        window.create(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
        fullscreen = true;
    }
    window.setFramerateLimit(60);
}

sf::Vector2i
CoreLoop::desktopCenterPosition()
{
    auto desktop = sf::VideoMode::getDesktopMode();
    return sf::Vector2i((desktop.width - windowMode.width) / 2, (desktop.height - windowMode.height) / 2);
}

void
CoreLoop::runCoreLoop()
{
    sf::Event event;

    while (window.isOpen())
    {
        window.clear();
        sceneManager.update();
        window.display();

        while (window.pollEvent(event))
        {
            process(event);
        }
    }
}

void
CoreLoop::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                case sf::Keyboard::F:
                    toggleFullscreen();
                    //imageView.resize();
                    break;

                default:
                    sceneManager.process(event);
                    break;
            }
            break;

        default:
            sceneManager.process(event);
            break;
    }
}
