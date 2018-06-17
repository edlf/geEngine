/*
 * Main.cpp
 *
 */
#include <Application.hpp>
#include <iostream>
#include <memory>

void PressEnterToExit() {
    std::cout << "Press enter to continue..." << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char** argv) {
    std::string sceneFileName;

    if (argc == 1) {
        sceneFileName.append("default.yaf");
        std::cout << "No scene defined, attempting to use [" << sceneFileName
                << "]." << std::endl;
    } else if (argc == 2) {
        sceneFileName.append(argv[1]);
        std::cout << "Attempting to use: [" << sceneFileName << "]."
                << std::endl;
    } else {
        std::cerr << "Too much arguments!" << std::endl;
        PressEnterToExit();
        return -1;
    }

    ge::Application application(argc, argv, sceneFileName);

    return 0;
}

