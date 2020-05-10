#include "ImageCache.h"
#include "Stage.h"
#include "SingleView.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    ImageCache imageCache(argv[1]);

    //sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(640, 480), "piv");
    window.setFramerateLimit(60);

    SingleView singleView(imageCache, window);
    Stage* stage = &singleView;

    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Q:
                            window.close();
                            break;
                        default:
                            stage->handle(event);
                            break;
                    }
                    break;

                default:
                    stage->handle(event);
                    break;
            }
        }

        imageCache.update();
        window.clear();
        stage->draw();
        window.display();
    }

    return EXIT_SUCCESS;
}
