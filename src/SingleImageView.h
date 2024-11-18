#pragma once

#include "ImageManager.h"
#include "ImageView.h"
#include "Image.h"
#include <SFML/Graphics.hpp>

class SingleImageView : public ImageView, public sf::Drawable
{
    public:
        SingleImageView(sf::RenderWindow& window, ImageManager& imageManager);

        void process(const sf::Event& event);
        void init(void);
        std::string helpMsg(void);

        std::deque<std::unique_ptr<Image>>::iterator imageIter;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        ImageManager& imageManager;
        sf::RenderWindow& window;

        sf::RectangleShape background;
        sf::Vector2i previousMousePosition;

        void next(void);
        void previous(void);
        void zoom(float delta);
        void toggleFit();
};
