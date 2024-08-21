#include "CoreLoop.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: piv <path>" << std::endl;
        return EXIT_FAILURE;
    }

    CoreLoop coreLoop(argv[1]);
    coreLoop.runCoreLoop();

    return EXIT_SUCCESS;
}
