#include "matrix.h"
#include "math.h"
#include <iostream>
#include <iomanip>

//Create matrix with integer array from user input string
//Will accept up to 256 integers and will ignore any remainders
matrix::matrix(std::stringstream& inputstream)
	:
	data(), size()
{
	int streamSize;
	int temp;
	int store[256];
	int capacity = 0;
	for (streamSize = 0; streamSize < 256; streamSize++) {
		if (inputstream >> temp) { //integer filter
			//Clamp to grayscale range 0...255
			if (temp > 255) { temp = 255;} 
			else if(temp < 0) { temp = 0;}
 			store[streamSize] = temp;
			capacity++;
		}
	}
	if (sqrt(double(capacity)) != floor(sqrt(double(capacity)))) {
		std::cerr << "Matrix requires N x N values.\n";
	}
	else {
		size = capacity;
		data = new int[size];
		dimension = sqrt(size);
		for (int i = 0; i < size; i++) {
			data[i] = store[i];
		}
		if (data == NULL) {
			std::cerr << "Warning: unable to find memory for data.\n";
		}
	}
}

//Copy constructor, deep copy of matrix data
matrix::matrix(const matrix* copy)
	:
	data(), size()
{
	if (copy == NULL) {
		std::cerr << "Constructor: Copy matrix returned NULL.\n";
		return;
	}
	size = copy->size;
	data = new int[size];
	dimension = sqrt(size);
	for (int i = 0; i < size; i++) {
		data[i] = copy->data[i];
	}
	if (data == NULL) {
		std::cerr << "Warning: unable to find memory for data.\n";
	}
}

//Free allocated memory
matrix::~matrix()
{
	delete data;
}

int matrix::getSize() const
{
	return size;
}

int matrix::getDimension() const
{
	return dimension;
}

//GetSet functions convert 2D coordinates to 1D.
//Return grayscale value at data[row, col]
int matrix::getAt(const int row, const int col) const
{
	if (data == NULL) {
		std::cerr << "Get: Data inaccessible.\n";
		return -1;
	}
	int coordinate = (row * dimension + col);
	if (0 <= coordinate && coordinate < size) {
		return data[coordinate];
	}
	else {
		std::cerr << "Could not get data at index " << coordinate << " with size " << size << "\n";
		return -1;
	}
}

//Set grayscale value at data[row, col] to value
void matrix::setAt(const int row, const int col, const int value)
{
	if (data == NULL) {
		std::cerr << "Set: Data inaccessible.\n";
		return;
	}
	int coordinate = (row * dimension + col);
	if (0 <= coordinate && coordinate <= size) {
		data[coordinate] = value;
	}
	else {
		std::cerr << "Could not set data at index " << coordinate << " with size " << size << "\n";
	}
}

//turnOn/Off are higher level functions of Get/Set
//Flip a pixel on or off
void matrix::turnOn(const int row, const int col)
{
	setAt(row, col, 1);
	return;
}

void matrix::turnOff(const int row, const int col)
{
	setAt(row, col, 0);
	return;
}

//rescale from 0...256 to 0...range
void matrix::rescale(const int range)
{
	if (data == NULL) {
		std::cerr << "Rescale: Data inaccessible.\n";
		return;
	}
	if (range < 0) {
		std::cerr << "Rescale: Invalid range.\n";
		return;
	}
	for (int i = 0; i < size; i++) {
		double divisor = 256 / (double(range) + 1);
		data[i] = data[i] / divisor;
	}
	return;
}

// Ordered dither operation on this matrix's data
void matrix::orderedDither(const matrix* ditherMatrix)
{
	if (ditherMatrix == NULL) {
		std::cerr << "Ordered Dither: Could not find dither matrix.\n";
		return;
	}
	rescale(4); //range specified for assignment
	for (int row = 0; row < dimension; row++) {
		for (int col = 0; col < dimension; col++) {
			//relative positions
			int relCol = col % ditherMatrix->dimension;
			int relRow = row % ditherMatrix->dimension;
			if (getAt(row, col) > ditherMatrix->getAt(relRow, relCol)) {
				turnOn(row, col);
			}
			else {
				turnOff(row, col);
			}
		}
	}
	return;
}

//Halftone printing operation on this matrix's data
void matrix::halftonePrint(const matrix* ditherMatrix)
{
	const int scaleFactor = ditherMatrix->dimension;
	if (ditherMatrix == NULL) {
		std::cerr << "Halftone Print: Could not find dither matrix.\n";
		return;
	}
	rescale(4); //range specified for assignment

	int scaledSize = size * ditherMatrix->size;
	int* scaledData = new int[scaledSize];	//create a larger scaled int array to replace old one
	int scaledDimension = dimension * scaleFactor;

	if (scaledData == NULL) {
		std::cerr << "Halftone Print: Error creating scaled data.\n";
		return;
	}
	//For each pixel in the old data array, compare to dither matrix
	//If data pixel is greater, then turn on corresponding pixel in scaled data array
	//Else turn off corresponding pixel in scaled data array
	for (int row = 0; row < dimension; row++) {
		for (int col = 0; col < dimension; col++) {
			int scaledRow = row * scaleFactor;
			int scaledCol = col * scaleFactor;
			if (getAt(row, col) > ditherMatrix->getAt(0, 0)) {
				scaledData[scaledRow * scaledDimension + scaledCol] = 1;
			}
			else {
				scaledData[scaledRow * scaledDimension + scaledCol] = 0;
			}
			if (getAt(row, col) > ditherMatrix->getAt(0, 1)) {
				scaledData[scaledRow * scaledDimension + scaledCol+1] = 1;
			}
			else {
				scaledData[scaledRow * scaledDimension + scaledCol + 1] = 0;
			}
			if (getAt(row, col) > ditherMatrix->getAt(1, 0)) {
				scaledData[(scaledRow+1) * scaledDimension + scaledCol] = 1;
			}
			else {
				scaledData[(scaledRow + 1) * scaledDimension + scaledCol] = 0;
			}
			if (getAt(row, col) > ditherMatrix->getAt(1, 1)) {
				scaledData[(scaledRow+1) * scaledDimension + scaledCol+1] = 1;
			}
			else {
				scaledData[(scaledRow + 1) * scaledDimension + scaledCol + 1] = 0;
			}
		}
	}

	delete data;
	data = scaledData;
	size = scaledSize;
	dimension = scaledDimension;
	return;
}

//Prints data contents to console
void matrix::print() const
{
	if (data == NULL) {
		std::cerr << "Print: Data inaccessible.\n";
		return;
	}
	for (int row = 0; row < dimension; row++) {
		std::cout << "[";
		for (int col = 0; col < dimension; col++) {
			if (col == dimension - 1) {
				std::cout << std::setw(3) << data[(row * dimension) + col] << "]\n";
			}
			else {
				std::cout << std::setw(3) << data[(row * dimension) + col] << " ";
			}
		}
	}
}
