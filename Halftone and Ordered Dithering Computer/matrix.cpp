#include "matrix.h"
#include "math.h"
#include <iostream>
#include <iomanip>

matrix::matrix(std::stringstream& inputstream)
	:
	data(), size()
{
	int streamSize;
	int temp;
	int store[1024];
	int capacity = 0;
	for (streamSize = 0; streamSize < 1024; streamSize++) {
		if (inputstream >> temp) { //integer filter
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

matrix::matrix(matrix* copy)
	:
	data(), size(copy->size)
{
	data = new int[size];
	dimension = sqrt(size);
	for (int i = 0; i < size; i++) {
		data[i] = copy->data[i];
	}
	if (data == NULL) {
		std::cerr << "Warning: unable to find memory for data.\n";
	}
}

matrix::matrix(int capacity) 
	:
	data(), size(capacity)
{
	if (0 <= size <= 1024) {
		data = new int[size];
	}
	if (data == NULL) {
		std::cerr << "Warning: unable to find memory for data.\n";
	}
}

matrix::~matrix()
{
	delete data;
}

int matrix::getSize()
{
	return size;
}

int matrix::getDimension()
{
	return dimension;
}

//GetSet works on 0....dimension-1 
int matrix::getAt(int row, int col)
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

void matrix::setAt(int row, int col, int value)
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

void matrix::turnOn(int row, int col)
{
	setAt(row, col, 1);
	return;
}

void matrix::turnOff(int row, int col)
{
	setAt(row, col, 0);
	return;
}

//rescale from 256 to 0...range
void matrix::rescale(int range)
{
	if (data == NULL) {
		std::cerr << "Rescale: Data inaccessible.\n";
		return;
	}
	for (int i = 0; i < size; i++) {
		double divisor = 256 / (double(range) + 1);
		data[i] = data[i] / divisor;
	}
	return;
}

void matrix::orderedDither(matrix* ditherMatrix)
{
	if (ditherMatrix == NULL) {
		std::cerr << "Ordered Dither: Could not find dither matrix.\n";
		return;
	}
	rescale(4);
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

void matrix::halftonePrint(matrix* ditherMatrix)
{
	const int scaleFactor = ditherMatrix->dimension;
	if (ditherMatrix == NULL) {
		std::cerr << "Halftone Print: Could not find dither matrix.\n";
		return;
	}
	rescale(4);

	int scaledSize = size * ditherMatrix->size;
	int* scaledData = new int[scaledSize];
	int scaledDimension = dimension * scaleFactor;

	if (scaledData == NULL) {
		std::cerr << "Halftone Print: Error creating scaled data.\n";
		return;
	}
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

//coordinate: xRow + yColumn
void matrix::print()
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
				std::cout << std::setw(3) << data[(row * dimension) + col] << ", ";
			}
		}
	}
}
