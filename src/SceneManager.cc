#include "SceneManager.h"
#include "Help.h"

SceneManager::SceneManager(Folder& folder, sf::RenderWindow& window)
    :
        window(window),
        gui(window),
        imageManager(folder),
        multiImageView(window, imageManager, gui),
        singleImageView(window, imageManager, gui)
{
    eventReceiver = &multiImageView;

    if (folder.selectedFolderExistsNotEmpty())
    {
        gui.showSelectedFolderWarning = true;
    }

    auto help = Help::general();
    help.append(Help::allImages());
    help.append(Help::selectedImages());
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
                case sf::Keyboard::M:
                    deactivateSingleImageView();
                    break;
                case sf::Keyboard::I:
                    gui.showInfo = !gui.showInfo;
                    break;
                default:
                    eventReceiver->process(event);
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
                    eventReceiver->process(event);
                    break;
            }
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Button::Left:
                    activateSingleImageView();
                    break;
                case sf::Mouse::Button::Right:
                    deactivateSingleImageView();
                    break;
                default:
                    eventReceiver->process(event);
                    break;
            }
            break;

        default:
            eventReceiver->process(event);
            break;
    }
}

void
SceneManager::activateSingleImageView()
{
    if (!singleImageViewActive)
    {
        singleImageViewActive = true;
        eventReceiver = &singleImageView;
        singleImageView.imageIter = std::find(imageManager.images.begin(), imageManager.images.end(), multiImageView.findImageUnderMouse());
        (*singleImageView.imageIter)->fitTo(window.getDefaultView());

        auto help = Help::general();
        help.append(Help::singleImage());
        gui.helpMsg(help);
    }
}

void
SceneManager::deactivateSingleImageView()
{
    if (singleImageViewActive)
    {
        (*singleImageView.imageIter)->resetPositionAndScale();
        singleImageViewActive = false;
        eventReceiver = &multiImageView;

        auto help = Help::general();
        help.append(Help::allImages());
        help.append(Help::selectedImages());
        gui.helpMsg(help);
    }
}

void
SceneManager::update()
{
    imageManager.update();
    multiImageView.draw();

    window.setView(window.getDefaultView());

    if (singleImageViewActive)
    {
        window.draw(singleImageView);
    }

    gui.update();
    window.draw(gui);
}
