#pragma once

#include <SFML/Graphics.hpp>

class Image {
    public:
        Image(const std::string& filename);

        sf::Sprite& getSprite(void);
        void fitToScreen(void);
        void zoom(float delta);
        void move(sf::Vector2i delta);

    private:
        sf::Texture texture;
        sf::Sprite sprite;

        sf::Vector2f mousePositionInSprite(void);
        sf::Vector2i mouseVector(void);
};
