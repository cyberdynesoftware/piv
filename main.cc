#include "Stage.h"
#include "SingleView.h"
#include "ScrollView.h"
#include "Folder.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode(800, 600), "piv");
    window.setFramerateLimit(30);
    bool fullscreen = false;

    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    SingleView singleImageView(folder, window);
    ScrollView multiImageView(folder, window);
    Stage* imageView = &multiImageView;
    //singleImageView.initImage();

    sf::Event event;

    while (window.isOpen())
    {
        window.clear();
        imageView->draw();
        window.display();

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    {
                        sf::View view = window.getView();
                        view.setSize(event.size.width, event.size.height);
                        view.setCenter(event.size.width / 2, event.size.height / 2);
                        window.setView(view);
                    }
                    imageView->resizeEvent();
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Q:
                            window.close();
                            break;

                        case sf::Keyboard::M:
                            if (imageView == &singleImageView)
                            {
                                multiImageView.scrollToCurrentImage();
                                window.setTitle("piv");
                                imageView = &multiImageView;
                            }
                            break;

                        case sf::Keyboard::F:
                            {
                                if (fullscreen)
                                {
                                    window.create(sf::VideoMode(800, 600), "piv");
                                    fullscreen = false;
                                }
                                else
                                {
                                    window.create(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
                                    fullscreen = true;
                                }
                                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                                imageView->resizeEvent();
                            }
                            break;

                        default:
                            imageView->handle(event);
                            break;
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Button::Left:
                            if (imageView == &multiImageView)
                            {
                                multiImageView.selectImage();
                                singleImageView.initImage();
                                imageView = &singleImageView;
                            }
                            else imageView->handle(event);
                            break;

                        case sf::Mouse::Button::Right:
                            if (imageView == &singleImageView)
                            {
                                multiImageView.scrollToCurrentImage();
                                window.setTitle("piv");
                                imageView = &multiImageView;
                            }
                            break;

                        default:
                            imageView->handle(event);
                            break;
                    }
                    break;

                default:
                    imageView->handle(event);
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
