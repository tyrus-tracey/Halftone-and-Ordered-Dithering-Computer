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
            matrix* copyMatrix = new matrix(inputMatrix);
            matrix* rescaleMat = new matrix(inputMatrix);
            std::cout << "Input: \n";
            inputMatrix->print();
            std::cout << "Rescale: \n";
            rescaleMat->rescale(4);
            rescaleMat->print();
            std::cout << "Halftone Printing: \n";
            copyMatrix->halftonePrint(ditherMatrix);
            copyMatrix->print();
            std::cout << "Ordered Dithering: \n";
            inputMatrix->orderedDither(ditherMatrix);
            inputMatrix->print();

            delete inputMatrix;
            delete copyMatrix;
            delete rescaleMat;
        }
    }
    delete ditherMatrix;
}