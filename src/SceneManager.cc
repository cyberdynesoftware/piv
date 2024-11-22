#include "SceneManager.h"
#include "Notification.h"

SceneManager::SceneManager(Folder& folder, sf::RenderWindow& window)
    :
        window(window),
        gui(window),
        imageManager(folder),
        multiImageView(window, imageManager, camera),
        singleImageView(window, imageManager)
{
    eventReceiver = &multiImageView;
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
                    gui.showHelpMsg(eventReceiver->helpMsg());
                    break;
                case sf::Keyboard::M:
                    deactivateSingleImageView();
                    break;
                case sf::Keyboard::I:
                    eventReceiver->showInfo = !eventReceiver->showInfo;
                    break;
                case sf::Keyboard::U:
                    toggleSortOrder();
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
                    gui.dismissHelpMsg();
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
        gui.drawProgressBar(multiImageView.calcProgress());
    }

    if (singleImageViewActive)
    {
        window.draw(singleImageView);
    }

    gui.update(clock.getElapsedTime());
    window.draw(gui);
}

void
SceneManager::toggleSortOrder()
{
    if (!singleImageViewActive)
    {
        auto intEnum = static_cast<int>(sortOrder);
        sortOrder = static_cast<ImageManager::SORT_ORDER>(++intEnum);

        if (sortOrder == ImageManager::SORT_ORDER::ENUM_GUARD)
        {
            sortOrder = ImageManager::SORT_ORDER::NAME;
        }

        multiImageView.reset();
        imageManager.changeSortOrder(sortOrder);
        camera.teleport(camera.view.getSize().y / 2.f);

        gui.notification = std::make_unique<Notification>();
        gui.notification->setMessage("Sort order: " + sortOrderToString(sortOrder))
            .setColor(sf::Color::White)
            .setSize(18)
            .setWidth(window.getDefaultView().getSize().x)
            .setPosition(sf::Vector2f(0, 0));

        gui.notification->timeout = sf::seconds(2.f);
        gui.notification->timeStamp = clock.getElapsedTime();
    }
}

std::string
SceneManager::sortOrderToString(ImageManager::SORT_ORDER sortOrder) const
{
    switch (sortOrder)
    {
        case ImageManager::SORT_ORDER::NAME: return "Name";
        case ImageManager::SORT_ORDER::RANDOM: return "Random";
        case ImageManager::SORT_ORDER::OLDEST: return "Oldest";
        case ImageManager::SORT_ORDER::NEWEST: return "Newest";
        default: return "?";
    }
}
