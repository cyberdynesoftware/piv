#ifndef __ANIMATEDGIF_H__
#define __ANIMATEDGIF_H__

#include "AnimatedImage.h"
#include <SFML/Graphics.hpp>
#include <future>

class AnimatedGIF : public AnimatedImage
{
    public:
        AnimatedGIF(const char*);
        ~AnimatedGIF();

        bool isGIF(void);
        void prepare(sf::Sprite&);
        void update(sf::Sprite&);

    private:
        struct stbi_pimpl* pimpl;
        char* fileBuffer;
        int filesize = 0;
        std::future<void> future;
        sf::Texture texture1;
        sf::Texture texture2;
        bool textureSelector = false;

        void loadFile(const char*);
        void loadPixels(sf::Texture*);
};

#endif
