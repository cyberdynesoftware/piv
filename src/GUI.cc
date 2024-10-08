#include "GUI.h"
#include <format>

GUI::GUI(sf::RenderWindow& window)
    :
        window(window)
{
    progressBar.setFillColor(sf::Color(255, 255, 255, 128));

    warning.setTextAndWidth("Warning: folder 'piv-selected' exists and is not empty.", window.getDefaultView().getSize().x);
}

void
GUI::update()
{
    if (showSelectedFolderWarning)
    {
        if (selectedFolderWarningClock.getElapsedTime() > sf::seconds(5.f))
        {
            showSelectedFolderWarning = false;
        }
    }
}

void
GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    window.setView(window.getDefaultView());

    if (showSelectedFolderWarning)
    {
        target.draw(warning, states);
    }

    if (showHelp)
    {
        target.draw(help, states);
    }
}

void
GUI::drawProgressBar(int index, int max)
{
    auto progress =  (float) index / max;
    auto msg = std::format("{} / {}", index, max);

    auto progressBarWidth = 15;
    progressBar.setSize(sf::Vector2f(progressBarWidth, progress * window.getDefaultView().getSize().y));
    progressBar.setPosition(window.getDefaultView().getSize().x - progressBarWidth, 0);

    progressLabel.setTextAndPadding(msg, sf::Vector2f(10.f, 10.f));
    progressLabel.setCenterPosition(sf::Vector2f(window.getDefaultView().getSize().x - progressBarWidth - 40,
                progress * window.getDefaultView().getSize().y));

    window.setView(window.getDefaultView());
    window.draw(progressBar);
    window.draw(progressLabel);
}

void
GUI::helpMsg(std::string msg)
{
    help.setTextAndPadding(msg, sf::Vector2f(50.f, 20.f));
    help.setCenterPosition(window.getDefaultView().getCenter());
}
