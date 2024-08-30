#include "SceneManager.h"

SceneManager::SceneManager(Folder& folder, sf::RenderWindow& window)
    :
        window(window),
        imageManager(folder),
        multiImageView(folder, window, imageManager),
        eventReceiver(multiImageView)
{
}

void
SceneManager::process(const sf::Event& event)
{
    eventReceiver.process(event);
}

void
SceneManager::update()
{
    imageManager.update();
    multiImageView.draw();
}
