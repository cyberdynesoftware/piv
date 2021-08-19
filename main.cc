#include "ImageView.h"
#include "SingleImageView.h"
#include "MultiImageView.h"
#include "Folder.h"
#include "icon.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode(800, 600), "piv", sf::Style::None);
    window.setPosition(sf::Vector2i(100, 100));
    window.setFramerateLimit(60);
    bool fullscreen = false;

    //sf::Image icon = Icon::createIcon();
    sf::Image icon;
    icon.loadFromMemory(icon_png, icon_png_len);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    MultiImageView imageView(folder, window);

    sf::Event event;

    while (window.isOpen())
    {
        window.clear();
        imageView.draw();
        window.display();

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                        case sf::Keyboard::Q:
                            window.close();
                            break;

                        case sf::Keyboard::F:
                            if (fullscreen)
                            {
                                window.create(sf::VideoMode(800, 600), "piv", sf::Style::None);
                                window.setPosition(sf::Vector2i(100, 100));
                                window.setFramerateLimit(60);
                                fullscreen = false;
                                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                            }
                            else
                            {
                                window.create(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
                                window.setFramerateLimit(60);
                                fullscreen = true;
                            }
                            imageView.resize();
                            break;

                        default:
                            imageView.handle(event);
                            break;
                    }
                    break;

                default:
                    imageView.handle(event);
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
