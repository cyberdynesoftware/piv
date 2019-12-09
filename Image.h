#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SFML/Graphics.hpp>

class Image {
    public:
        Image(const std::string &filename);

        sf::Sprite& getSprite(void);
        void fitToScreen(void);
        void zoom(sf::Event::MouseWheelScrollEvent& scrollEvent);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
};

#endif
