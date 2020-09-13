#include "Grid.h"
#include <iostream>
#include <algorithm>



void Grid::getPoint(int i, float* p)
{
	p[0] = pointsX[i];
	p[1] = pointsY[i];
	p[2] = pointsZ[i];
}


void Grid::setPoint(int i, vector<float> p)
{
	pointsX[i] = p[0];
	pointsY[i] = p[1];
	pointsZ[i] = p[2];
}

int	Grid::getCell(int cell, int* vertices)
{
	vertices[0] = cells[3 * cell];
	vertices[1] = cells[3 * cell + 1];
	vertices[2] = cells[3 * cell + 2];

	return 3;
}

void Grid::setCell(int cell, vector<int> vertices)
{
	cells[3 * cell] = vertices[0];
	cells[3 * cell + 1] = vertices[1];
	cells[3 * cell + 2] = vertices[2];
}

