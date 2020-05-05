#pragma once

#include "Stage.h"
#include "Folder.h"

class SingleView : public Stage
{
    public:
        SingleView(Folder&, sf::RenderWindow&);

        void handle(sf::Event&);

    private:
        Folder& folder;
        sf::RenderWindow& window;
        sf::Cursor arrow;
        sf::Cursor cross;
        sf::Vector2i previousMousePosition;
};
