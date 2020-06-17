#pragma once
#include <sstream>

class matrix
{
public:
	matrix(std::stringstream& inputstream);
	matrix(matrix* copy);
	~matrix();
	int getSize();
	int getDimension();
	int getAt(int row, int col);
	void setAt(int row, int col, int value);
	void turnOn(int row, int col);
	void turnOff(int row, int col);

	void rescale(int range);
	void orderedDither(matrix* ditherMatrix);
	void halftonePrint(matrix* ditherMatrix);
	void print();

private:
	int* data;
	int size = 0;
	int dimension = 0;
};

