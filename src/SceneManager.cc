#include "SceneManager.h"
#include "Notification.h"
#include "Help.h"
#include <iostream>

SceneManager::SceneManager(Folder& folder, sf::RenderWindow& window)
    :
        window(window),
        gui(window),
        imageManager(folder),
        multiImageView(window, imageManager, camera),
        singleImageView(window, imageManager)
{
    eventReceiver = &multiImageView;

    auto help = Help::general();
    help.append(Help::allImages());
    help.append(Help::selectedImages());
    gui.helpMsg(help);
}

void
SceneManager::checkFolder(const Folder& folder)
{
    if (folder.selectedFolderExistsNotEmpty())
    {
        gui.notification = std::make_unique<Notification>();
        gui.notification->setMessage("Warning: folder 'piv-selected' exists and is not empty.")
            .setColor(sf::Color::Red)
            .setSize(20)
            .setWidth(window.getDefaultView().getSize().x)
            .setPosition(sf::Vector2f(0, 0));

        gui.notification->timeout = sf::seconds(4.f);
        gui.notification->timeStamp = clock.getElapsedTime();
    }
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
                    eventReceiver->showInfo = !eventReceiver->showInfo;
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

        case sf::Event::Resized:
            multiImageView.resize(event.size.width, event.size.height);

            if (singleImageViewActive)
            {
                singleImageView.init();
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
        auto targetImage = multiImageView.findImageUnderMouse();
        if (targetImage != imageManager.images.end())
        {
            singleImageViewActive = true;
            eventReceiver = &singleImageView;
            singleImageView.imageIter = targetImage;
            singleImageView.init();

            auto help = Help::general();
            help.append(Help::singleImage());
            gui.helpMsg(help);
        }
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
    imageManager.update(clock.getElapsedTime());

    window.setView(camera.view);

    multiImageView.draw();

    window.setView(window.getDefaultView());

    if (camera.update(clock.getElapsedTime()))
    {
        auto progress = multiImageView.calcProgress();
        gui.drawProgressBar(progress.first, progress.second);
    }

    if (singleImageViewActive)
    {
        window.draw(singleImageView);
    }

    gui.update(clock.getElapsedTime());
    window.draw(gui);
}
