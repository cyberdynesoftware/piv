#include "ImageCache.h"
#include "Stage.h"
#include "SingleView.h"
#include "ScrollView.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    //ImageCache imageCache(argv[1]);
    Folder folder(argv[1]);

    sf::RenderWindow window(sf::VideoMode(800, 600), "piv");
    window.setFramerateLimit(60);
    bool fullscreen = false;

    SingleView singleView(folder, window);
    //ScrollView scrollView(imageCache, window);
    Stage* stage = &singleView;
    singleView.init();

    sf::View defaultView = window.getView();
    sf::Cursor arrow;
    arrow.loadFromSystem(sf::Cursor::Arrow);
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    defaultView.setSize(event.size.width, event.size.height);
                    defaultView.setCenter(event.size.width / 2, event.size.height / 2);
                    window.setView(defaultView);
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Q:
                            window.close();
                            break;
                            /*
                        case sf::Keyboard::M:
                            if (stage->instanceOf(Stage::SubType::SingleView))
                            {
                                scrollView.scrollToCurrentImage();
                                stage = &scrollView;
                                break;
                            }
                            */
                        case sf::Keyboard::F:
                            if (fullscreen)
                            {
                                window.create(sf::VideoMode(800, 600), "piv", sf::Style::Default);
                                defaultView.setSize(800, 600);
                                defaultView.setCenter(400, 300);
                                window.setView(defaultView);
                                fullscreen = false;
                            }
                            else
                            {
                                auto mode = sf::VideoMode::getDesktopMode();
                                window.create(mode, "piv", sf::Style::Fullscreen);
                                defaultView.setSize(mode.width, mode.height);
                                defaultView.setCenter(mode.width / 2, mode.height / 2);
                                window.setView(defaultView);
                                fullscreen = true;
                            }
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
                                //scrollView.selectImage();
                                singleView.init();
                                stage = &singleView;
                                window.setView(defaultView);
                                break;
                            }
                            window.setMouseCursor(arrow);
                        default:
                            break;
                    }
                    break;

                default:
                    stage->handle(event);
                    break;
            }
        }

        //imageCache.update();
        window.clear();
        stage->draw();
        window.display();
    }

    return EXIT_SUCCESS;
}
