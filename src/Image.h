#pragma once

#include <SFML/Graphics.hpp>
#include <future>
#include "Folder.h"

class Image
{
    public:
        virtual void init(const std::string& path);
        virtual void prepare(void);
        virtual void load(const sf::Time& time) {}
        virtual void update(const sf::Time& time) {}

        void setPosition(const sf::Vector2f&);
        void fitTo(const sf::View&);
        void scaleTo(int targetWidth);
        void resetPositionAndScale();

        sf::Sprite sprite;
        sf::Vector2u size;
        sf::Vector2f position;

        bool valid = false;
        bool hasPosition = false;
        bool selected = false;

        std::string info;
        std::string path;

    protected:
        void prepareInfo(const std::string& decoder);

    private:
        sf::Texture texture;
        float scale = 1.f;
};
