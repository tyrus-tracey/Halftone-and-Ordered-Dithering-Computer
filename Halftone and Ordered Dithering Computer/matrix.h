#pragma once
#include <sstream>
/*
	Stores integer array up to 256 elements (exception: halftone print scaling) representing image.
	Member functions manipulate and display integer array.
*/

class matrix
{
public:
	matrix(std::stringstream& inputstream);
	matrix(const matrix* copy);
	~matrix();
	int getSize() const;
	int getDimension() const;
	int getAt(const int row, const int col) const;
	void setAt(const int row, const int col, const int value);

	void turnOn(const int row, const int col);
	void turnOff(const int row, const int col);
	void rescale(const int range);
	void orderedDither(const matrix* ditherMatrix);
	void halftonePrint(const matrix* ditherMatrix);
	void print() const;
private:
	int* data;
	int size = 0;
	int dimension = 0;
};

