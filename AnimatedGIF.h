#ifndef __ANIMATEDGIF_H__
#define __ANIMATEDGIF_H__

#include <SFML/Graphics.hpp>

class AnimatedGIF
{
    public:
        AnimatedGIF();

        static bool test(const char*);
        void load(const char*);

        const sf::Time& delay(void);
        void update(sf::Texture&);

        sf::Vector2i size;
        int frameCount = 0;

    private:
        std::vector<std::tuple<sf::Time, sf::Image>> frames;
        std::vector<std::tuple<sf::Time, sf::Image>>::iterator frameIter;
};

#endif
