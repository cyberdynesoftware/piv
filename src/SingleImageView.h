#pragma once

#include "ImageManager.h"
#include "GUI.h"
#include "ImageView.h"
#include "Image.h"
#include <SFML/Graphics.hpp>

class SingleImageView : public ImageView, public sf::Drawable
{
    public:
        SingleImageView(sf::RenderWindow& window, ImageManager& imageManager, GUI& gui);

        void process(const sf::Event& event);
        void resizeEvent(void);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        ImageManager& imageManager;
        GUI& gui;
        sf::RenderWindow& window;

        sf::RectangleShape background;
        sf::Vector2i previousMousePosition;

        void next(void);
        void previous(void);
        void zoom(float delta);
        void toggleFit();
};
