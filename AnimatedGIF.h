#ifndef __ANIMATEDGIF_H__
#define __ANIMATEDGIF_H__

#include <SFML/Graphics.hpp>

class AnimatedGIF
{
    public:
        AnimatedGIF();

        static bool test(const char*);
        void load(const char*);
        const sf::Vector2i& getSize(void);
        void update(sf::Sprite&, bool);

    private:
        sf::Vector2i size;
        sf::Clock clock;
        sf::Time startTime;
        sf::Time totalDelay;
        std::vector<std::tuple<sf::Time, sf::Texture>> frames;
        std::vector<std::tuple<sf::Time, sf::Texture>>::iterator frameIter;
};

#endif
