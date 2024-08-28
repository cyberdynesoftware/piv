#include "CoreLoop.h"
#include "icon.h"

CoreLoop::CoreLoop(char* path)
    : 
        window(sf::VideoMode(1280, 720), "piv", sf::Style::Default),
        folder(path),
        imageView(folder, window),
        inputEvent(*this, imageView)
{
    window.setPosition(windowPos());
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
        setupWindow();
        fullscreen = false;
    }
    else
    {
        setupFullscreen();
        fullscreen = true;
    }
    window.setFramerateLimit(60);
}

void
CoreLoop::setupFullscreen()
{
    auto desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "piv", sf::Style::Fullscreen);
}

void
CoreLoop::setupWindow()
{
    window.create(windowMode, "piv", sf::Style::Default);
    window.setPosition(windowPos());
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

sf::Vector2i
CoreLoop::windowPos()
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
        imageView.draw();
        window.display();

        while (window.pollEvent(event))
        {
            inputEvent.process(event);
        }
    }
}
