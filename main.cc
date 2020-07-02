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

    ImageCache imageCache(argv[1]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "piv", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    SingleView singleView(imageCache, window);
    ScrollView scrollView(imageCache, window);
    Stage* stage = &scrollView;

    sf::View defaultView = window.getView();
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
                        case sf::Keyboard::M:
                            if (stage->instanceOf(Stage::SubType::SingleView))
                            {
                                scrollView.scrollToCurrentImage();
                                stage = &scrollView;
                                break;
                            }
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
                                singleView.init();
                                stage = &singleView;
                                window.setView(defaultView);
                                break;
                            }
                        default:
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
