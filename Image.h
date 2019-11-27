#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SFML/Graphics.hpp>

class Image {
    public:
        Image(const std::string &filename);

        sf::Sprite& getSprite(void);

    private:
        sf::Texture texture;
        sf::Sprite sprite;

        void fitToScreen(void);
};

#endif
