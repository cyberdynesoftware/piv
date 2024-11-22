#include "GUI.h"
#include <format>
#include <iostream>

GUI::GUI(sf::RenderWindow& window)
    :
        window(window)
{
    progressBar.setFillColor(sf::Color(255, 255, 255, 128));
}

void
GUI::update(const sf::Time& time)
{
    if (notification && time > notification->timeStamp + notification->timeout)
    {
        notification = nullptr;
    }
}

void
GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (notification)
    {
        target.draw(*notification, states);
    }

    if (dialog)
    {
        target.draw(*dialog, states);
    }
}

void
GUI::drawProgressBar(std::pair<int, int> progress)
{
    auto progressFraction =  (float) progress.first / progress.second;

    auto progressBarWidth = 15;
    progressBar.setSize(sf::Vector2f(progressBarWidth, progressFraction * window.getDefaultView().getSize().y));
    progressBar.setPosition(window.getDefaultView().getSize().x - progressBarWidth, 0);

    auto windowSize = window.getDefaultView().getSize();
    progressBarLabel.setMessage(std::format("{} / {}", progress.first, progress.second))
        .setPosition(sf::Vector2f(windowSize.x, windowSize.y * progressFraction), progressFraction);

    window.draw(progressBar);
    window.draw(progressBarLabel);
}

void
GUI::showHelpMsg(std::string msg)
{
    dialog = std::make_unique<Dialog>();
    dialog->setMessage(msg)
        .setColor(sf::Color::White)
        .setSize(18)
        .setPadding(sf::Vector2f(50.f, 20.f))
        .setCenter(window.getDefaultView().getCenter());
}

void
GUI::dismissHelpMsg()
{
    dialog = nullptr;
}
