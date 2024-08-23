#include "GUI.h"
#include "font.h"

GUI::GUI()
{
    font.loadFromMemory(font_ttf, font_ttf_len);
    progressBar.setFillColor(sf::Color(255, 255, 255, 128));
}

void
GUI::drawInfoBox(sf::RenderWindow& window, Image* image)
{
    if (showInfo) 
    {
        sf::Text info;
        info.setFont(font);
        info.setFillColor(sf::Color::White);
        info.setCharacterSize(15);
        info.setString(image->info);

        const auto& infoBounds = info.getLocalBounds();
        const auto& imageBounds = image->sprite.getGlobalBounds();
        sf::RectangleShape background(sf::Vector2f(imageBounds.width, infoBounds.top * 2 + infoBounds.height));
        background.setFillColor(sf::Color(0, 0, 0, 64));

        background.setPosition(imageBounds.left, imageBounds.top);
        info.setPosition(imageBounds.left, imageBounds.top);

        window.draw(background);
        window.draw(info);
    }
}

void
GUI::drawSelectedIcon(sf::RenderWindow& window, Image* image)
{
    sf::CircleShape circle;
    circle.setRadius(6);
    circle.setFillColor(sf::Color::Cyan);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);

    const auto& imageBounds = image->sprite.getGlobalBounds();
    circle.setPosition(imageBounds.left + imageBounds.width - 20, imageBounds.top + 8);
    window.draw(circle);
}

void
GUI::drawProgressBar(sf::RenderWindow& window, float progress, std::string msg)
{
    progressBar.setSize(sf::Vector2f(progressBarWidth, progress * window.getView().getSize().y));
    progressBar.setPosition(window.getView().getSize().x - progressBarWidth, window.getView().getCenter().y - window.getView().getSize().y / 2);
    window.draw(progressBar);

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(15);
    text.setString(msg);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);

    sf::RectangleShape background(sf::Vector2f(text.getLocalBounds().width + 20, text.getLocalBounds().height + 20));
    background.setFillColor(sf::Color(255, 255, 255, 128));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.f);
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
    float x = window.getView().getSize().x - progressBarWidth - background.getSize().x / 2.f - 5;
    float y = window.getView().getCenter().y - window.getView().getSize().y / 2.f + progress * window.getView().getSize().y;
    background.setPosition(x, y);
    text.setPosition(x - text.getLocalBounds().left, y - text.getLocalBounds().top);

    window.draw(background);
    window.draw(text);
}

void
GUI::drawHelpText(sf::RenderWindow& window, std::string help)
{
    if (showHelp)
    {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(18);
        text.setString(help);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(window.getView().getCenter());

        sf::RectangleShape background(sf::Vector2f(text.getLocalBounds().width + 50, text.getLocalBounds().height + 20));
        background.setFillColor(sf::Color(0, 0, 0, 192));
        background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
        background.setPosition(window.getView().getCenter());

        window.draw(background);
        window.draw(text);
    }
}

void
GUI::selectedFolderWarning(sf::RenderWindow& window)
{
    if (selectedFolderWarningCounter++ < 400)
    {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(20);
        text.setString("Warning: 'piv-selected' exists and is not empty.");
        text.setPosition(20, window.getView().getCenter().y - window.getView().getSize().y / 2 + 10);

        sf::RectangleShape background(sf::Vector2f(window.getView().getSize().x, text.getLocalBounds().height + 30));
        background.setFillColor(sf::Color(0, 0, 0, 192));
        background.setPosition(0, window.getView().getCenter().y - window.getView().getSize().y / 2);

        window.draw(background);
        window.draw(text);
    }
}
