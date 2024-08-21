#include "CoreLoop.h"
#include "icon.h"

CoreLoop::CoreLoop(char* path)
    : 
        window(sf::VideoMode(800, 600), "piv", sf::Style::Default),
        folder(path),
        imageView(folder, window),
        inputEvent(*this, imageView)
{
    window.setPosition(sf::Vector2i(80, 60));
    window.setFramerateLimit(60);

    icon.loadFromMemory(icon_png, icon_png_len);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void
CoreLoop::closeWindow()
{
    window.close();
}

void
CoreLoop::toggleFullscreen()
{
    if (fullscreen)
    {
        window.create(sf::VideoMode(800, 600), "piv", sf::Style::Default);
        window.setPosition(sf::Vector2i(100, 100));
        window.setFramerateLimit(60);
        fullscreen = false;
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    else
    {
        window.create(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
        window.setFramerateLimit(60);
        fullscreen = true;
    }
}

void
CoreLoop::runCoreLoop()
{
    sf::Event event;

    while (window.isOpen())
    {
        window.clear();
        imageView.draw();
        window.display();

        while (window.pollEvent(event))
        {
            inputEvent.process(event);
        }
    }
}
