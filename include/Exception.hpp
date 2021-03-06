/*
 * Eduardo Fernandes
 *
 * Exception class.
 */

#ifndef GEEXCEPTION_HPP_
#define GEEXCEPTION_HPP_

#include <iostream>
#include <string>

namespace ge {

class Exception {
private:
    std::string errorMessage;

    void PressEnterToContinue() {
        std::cerr << "Press enter to continue..." << std::flush;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

public:
    Exception() {
        errorMessage = "Unknown error.";
    }

    Exception(std::string str) {
        errorMessage = str;
    }

    Exception(std::string str, bool fatal) {
        errorMessage = str;

        if (fatal) {
            std::cerr << "Fatal exception caught: " << errorMessage.c_str()
                    << std::endl << std::flush;
            PressEnterToContinue();
            exit(-1);
        }

    }

    Exception(std::string str, bool fatal, int line) {
        errorMessage = str;

        if (fatal) {
            std::cerr << "Fatal exception while parsing xml at or near line: "
                    << line << std::endl;
            std::cerr << "Error: " << errorMessage.c_str() << std::endl;
            PressEnterToContinue();
            exit(-1);
        }

    }

    void printerErrorMessage() {
        std::cerr << "Exception caught: " << errorMessage << std::endl;
    }
};

}

#endif
