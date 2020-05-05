#ifndef __STAGE_H__
#define __STAGE_H__


#include <SFML/Graphics.hpp>

class Stage
{
    public:
        virtual void handle(sf::Event&) = 0;
};

#endif
