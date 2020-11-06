#ifndef __ANIMATEDGIF_H__
#define __ANIMATEDGIF_H__

#include <SFML/Graphics.hpp>

class AnimatedGIF
{
    public:
        AnimatedGIF(const char*);
        ~AnimatedGIF();

        bool isGIF(void);
        void update(sf::Texture&);

        sf::Time delay;

    private:
        struct stbi_pimpl* pimpl;
        char* fileBuffer;
        int filesize = 0;

        void loadFile(const char*);
};

#endif
