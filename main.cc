#include "ImageCache.h"
#include "Stage.h"
#include "ScrollView.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    ImageCache imageCache(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    ScrollView view(imageCache, window);
    Stage* stage = &view;

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
