#include "Stage.h"
#include "SingleView.h"
#include "ScrollView.h"
#include "Folder.h"

#include <iostream>
#include <OpenImageIO/imagecache.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    OIIO::ImageCache::create();
    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode(800, 600), "piv");
    window.setFramerateLimit(60);
    bool fullscreen = false;

    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    SingleView singleView(folder, window);
    ScrollView scrollView(folder, window);
    Stage* stage = &scrollView;
    //singleView.initImage();

    sf::Event event;

    while (window.isOpen())
    {
        window.clear();
        stage->draw();
        window.display();

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    stage->resizeEvent(event.size);
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Q:
                            window.close();
                            break;

                        case sf::Keyboard::M:
                            if (stage->instanceOf(Stage::SubType::SingleView))
                            {
                                scrollView.scrollToCurrentImage();
                                stage = &scrollView;
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
                            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                            break;

                        default:
                            stage->handle(event);
                            break;
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Button::Left:
                            if (stage->instanceOf(Stage::SubType::ScrollView))
                            {
                                scrollView.selectImage();
                                singleView.initImage();
                                stage = &singleView;
                            }
                            else stage->handle(event);
                            break;

                        case sf::Mouse::Button::XButton1:
                            if (stage->instanceOf(Stage::SubType::SingleView))
                            {
                                scrollView.scrollToCurrentImage();
                                stage = &scrollView;
                            }
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
    }

    return EXIT_SUCCESS;
}
