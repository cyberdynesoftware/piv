#include "GUI.h"

GUI::GUI()
{
    progressBar.setFillColor(sf::Color(255, 255, 255, 128));

    selectedIconCircle.setRadius(5);
    selectedIconCircle.setFillColor(pumpkin);
    selectedIconCircle.setOutlineColor(sf::Color::Black);
    selectedIconCircle.setOutlineThickness(1);
}

void
GUI::drawInfoBox(sf::RenderWindow& window, Image* image)
{
    if (showInfo) 
    {
        info.setTextAndWidth(image->info, image->sprite.getGlobalBounds().width);
        info.setPosition(image->sprite.getGlobalBounds().getPosition());
        window.draw(info);
    }
}

void
GUI::drawSelectedIcon(sf::RenderWindow& window, Image* image)
{
    const auto& imageBounds = image->sprite.getGlobalBounds();
    selectedIconCircle.setPosition(imageBounds.left + imageBounds.width - 20, imageBounds.top + 8);
    window.draw(selectedIconCircle);
}

void
GUI::drawProgressBar(sf::RenderWindow& window, float progress, std::string msg)
{
    int progressBarWidth = 15;
    progressBar.setSize(sf::Vector2f(progressBarWidth, progress * window.getView().getSize().y));
    progressBar.setPosition(window.getView().getSize().x - progressBarWidth, window.getView().getCenter().y - window.getView().getSize().y / 2);
    window.draw(progressBar);

    progressLabel.setTextAndPadding(msg, sf::Vector2f(20.f, 20.f));
    progressLabel.setCenterPosition(sf::Vector2f(window.getView().getSize().x - progressBarWidth - 40,
                window.getView().getCenter().y - window.getView().getSize().y / 2.f + progress * window.getView().getSize().y));

    window.draw(progressLabel);
}

void
GUI::drawHelpText(sf::RenderWindow& window, std::string msg)
{
    if (showHelp)
    {
        help.setTextAndPadding(msg, sf::Vector2f(50.f, 20.f));
        help.setCenterPosition(window.getView().getCenter());
        window.draw(help);
    }
}

void
GUI::selectedFolderWarning(sf::RenderWindow& window)
{
    // TODO: counting frames... *eyeroll*
    if (selectedFolderWarningCounter++ < 400)
    {
        warning.setTextAndWidth("Warning: folder 'piv-selected' exists and is not empty.", window.getView().getSize().x);
        warning.setPosition(sf::Vector2f(0.f, window.getView().getCenter().y - window.getView().getSize().y / 2));
        window.draw(warning);
    }
}
