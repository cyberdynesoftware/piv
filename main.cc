#include "Image.h"
#include "Folder.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.draw(folder.firstImage().getSprite());
    window.display();

    sf::Event event;

    while (window.isOpen())
    {
        window.waitEvent(event);

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::Q:
                    window.close();
                    break;
                case sf::Keyboard::Space:
                    window.clear();
                    window.draw(folder.nextImage().getSprite());
                    window.display();
                    break;
                case sf::Keyboard::Backspace:
                    window.clear();
                    window.draw(folder.prevImage().getSprite());
                    window.display();
                    break;
                default:
                    break;
            }
        }

    }

    return EXIT_SUCCESS;
}
