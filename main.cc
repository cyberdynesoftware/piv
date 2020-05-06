#include "Image.h"
#include "Folder.h"
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

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.draw(folder.getImage()->getSprite());
    window.display();

    SingleView singleView(folder, window);
    Stage* stage = &singleView;

    sf::Event event;

    while (window.isOpen())
    {
        window.waitEvent(event);

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

        window.clear();
        stage->draw();
        window.display();
    }

    return EXIT_SUCCESS;
}
