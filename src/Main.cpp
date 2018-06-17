/*
 * Main.cpp
 *
 */
#include <Application.hpp>
#include <iostream>
#include <memory>


void PressEnterToExit() {
    cout << "Press enter to continue..." << flush;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char** argv) {
    std::string sceneFileName;

    if (argc == 1) {
        sceneFileName.append("default.yaf");
        cout << "No scene defined, attempting to use [" << sceneFileName << "]." << endl;
    } else if (argc == 2) {
        sceneFileName.append(argv[1]);
        cout << "Attempting to use: [" << sceneFileName << "]." << endl;
    } else {
        cerr << "Too much arguments!" << endl;
        PressEnterToExit();
        return -1;
    }

    Application application(argc, argv, sceneFileName);

    return 0;
}

