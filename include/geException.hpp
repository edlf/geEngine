/*
 * Eduardo Fernandes
 *
 * Exception class.
 */

#ifndef GEEXCEPTION_HPP_
#define GEEXCEPTION_HPP_

#include <iostream>
#include <string>

using namespace std;

namespace ge {

class geException {
private:
    string errorMessage;

    void PressEnterToContinue() {
        cerr << "Press enter to continue..." << flush;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

public:
    geException() {
        errorMessage = "Unknown error.";
    }

    geException(string str) {
        errorMessage = str;
    }

    geException(string str, bool fatal) {
        errorMessage = str;

        if (fatal) {
            cerr << "Fatal exception caught: " << errorMessage.c_str() << endl << flush;
            PressEnterToContinue();
            exit(-1);
        }

    }

    geException(string str, bool fatal, int line) {
        errorMessage = str;

        if (fatal) {
            cerr << "Fatal exception while parsing xml at or near line: " << line << endl;
            cerr << "Error: " << errorMessage.c_str() << endl;
            PressEnterToContinue();
            exit(-1);
        }

    }

    void printerErrorMessage() {
        cerr << "Exception caught: " << errorMessage.c_str() << endl;
    }
};

}

#endif
