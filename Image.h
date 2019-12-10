#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SFML/Graphics.hpp>

class Image {
    public:
        Image(const std::string &filename);

        sf::Sprite& getSprite(void);
        void fitToScreen(void);
        void zoom(sf::Event::MouseWheelScrollEvent& scrollEvent);
        void move(int deltaX, int deltaY);

    private:
        sf::Texture texture;
        sf::Sprite sprite;

        sf::Vector2f mousePositionInSprite(void);
        sf::Vector2i mouseVector(void);
};

#endif
