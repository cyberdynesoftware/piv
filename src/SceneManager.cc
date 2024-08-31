#include "SceneManager.h"

SceneManager::SceneManager(Folder& folder, sf::RenderWindow& window)
    :
        window(window),
        gui(window),
        imageManager(folder),
        multiImageView(window, imageManager, gui),
        eventReceiver(multiImageView)
{
    if (folder.selectedFolderExistsNotEmpty())
    {
        gui.showSelectedFolderWarning = true;
    }

    auto help = Help::general();
    help.append(Help::allImages());
    gui.helpMsg(help);
}

void
SceneManager::process(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::H:
                    gui.showHelp = true;
                    break;
                default:
                    eventReceiver.process(event);
                    break;
            }
            break;

        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
                case sf::Keyboard::H:
                    gui.showHelp = false;
                    break;
                default:
                    eventReceiver.process(event);
                    break;
            }
            break;

        default:
            eventReceiver.process(event);
            break;
    }
}

void
SceneManager::update()
{
    imageManager.update();
    multiImageView.draw();
    gui.update();
    window.draw(gui);
}
