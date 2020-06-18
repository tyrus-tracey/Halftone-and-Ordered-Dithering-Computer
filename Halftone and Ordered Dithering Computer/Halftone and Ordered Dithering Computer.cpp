#include <iostream>
#include <sstream>
#include <string>
#include "matrix.h"

bool running = true;
std::string input;
std::stringstream ditherStream("0 3 2 1");
const matrix* ditherMatrix = new matrix(ditherStream);

int main()
{
    std::cout << "Halftone printing and Ordered dithering computer\n";
    if (ditherMatrix == NULL) {
        std::cerr << "Error: No dither matrix.\n";
        return 1;
    }
    while (running) {
        std::cout << "\nEnter N x N integers of grayscale values or X to close: \n";
        std::getline(std::cin, input); // Receive user input
        if (input == "X" || input == "x") {
            running = false;
        }
        else {
            std::stringstream inputstream(input); // Create stream from input string
            matrix* inputMatrix = new matrix(inputstream); //Resulting matrix from input
            matrix* copyMatrix = new matrix(inputMatrix); //Copy matrix for second operation
            if (inputMatrix == NULL || copyMatrix == NULL) {
                std::cerr << "Error: No input matrices.\n";
                return 1;
            }
            
            std::cout << "Input: \n";
            inputMatrix->print();

            std::cout << "Halftone Printing: \n";
            copyMatrix->halftonePrint(ditherMatrix);
            copyMatrix->print();

            std::cout << "Ordered Dithering: \n";
            inputMatrix->orderedDither(ditherMatrix);
            inputMatrix->print();

            // End of operations on input, free memory 
            delete inputMatrix;
            delete copyMatrix;
        }
    }
    // End of program, free remaining memory
    delete ditherMatrix;
    return 0;
}