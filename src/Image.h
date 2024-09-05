#pragma once

#include <SFML/Graphics.hpp>
#include <future>
#include "Folder.h"

class Image
{
    public:
        virtual void init(const std::string& path);
        virtual void prepare(void);
        virtual void update(const sf::Time& time);

        void setPosition(const sf::Vector2f&);
        void fitTo(const sf::View&);
        void scaleTo(int targetWidth);
        void resetPositionAndScale();

        sf::Sprite sprite;
        bool valid = false;
        bool hasPosition = false;
        sf::Vector2f position;
        std::string info;
        std::string path;
        bool selected = false;

    protected:
        sf::Texture texture;

        void prepareInfo(const std::string& decoder);

    private:
        float scale = 1.f;
};
