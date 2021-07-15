#include "ImageView.h"
#include "SingleImageView.h"
#include "MultiImageView.h"
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
    window.setFramerateLimit(60);
    bool fullscreen = false;

    sf::Image icon;
    if (Folder::fileExists("icon.png"))
    {
        icon.loadFromFile("icon.png");
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    SingleImageView singleImageView(folder, window);
    MultiImageView multiImageView(folder, window);
    ImageView* imageView;
    if (folder.imageSelected)
    {
        imageView = &singleImageView;
        singleImageView.initImage();
    }
    else
    {
        imageView = &multiImageView;
    }

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
                    multiImageView.resizeEvent();
                    if (imageView == &singleImageView)
                        singleImageView.resizeEvent();
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
                            if (Folder::fileExists("icon.png"))
                                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                            multiImageView.resizeEvent();
                            if (imageView == &singleImageView)
                                singleImageView.resizeEvent();
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
                            if (imageView == &multiImageView && multiImageView.selectImage())
                            {
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
