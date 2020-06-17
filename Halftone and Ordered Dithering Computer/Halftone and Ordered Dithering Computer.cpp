#include <iostream>
#include <sstream>
#include <string>
#include "matrix.h"

bool running = true;
std::string input;
int inputArray[256];
std::stringstream ditherStream("0 3 2 1");
matrix* ditherMatrix = new matrix(ditherStream);

int main()
{
    std::cout << "Halftone printing and Ordered dithering computer\n";

    while (running) {
        std::cout << "Enter N x N integers of grayscale values or X to close: \n";
        std::getline(std::cin, input);
        if (input == "X") {
            running = false;
        }
        else {
            std::stringstream inputstream(input);
            matrix* inputMatrix = new matrix(inputstream);
            std::cout << "Input: \n";
            inputMatrix->print();
           // std::cout << "Rescale: \n";
            //inputMatrix->rescale(4);
            inputMatrix->orderedDither(ditherMatrix);
            std::cout << "Ordered Dithering: \n";
            inputMatrix->print();

            delete inputMatrix;
        }
    }
    delete ditherMatrix;
}