#pragma once

#include <vector>
#include <stdio.h>
#include <string>




class Grid
{
public:
	Grid(int P, int C)															//Resize the array for the points and cells for the grid
	{
		pointsX.resize(P);
		pointsY.resize(P);
		pointsZ.resize(P);
		cells.resize(3 * C);
	}

	int numPoints()
	{
		return pointsX.size();
	}

	int numCells()
	{
		return cells.size() / 3;
	}

	void getPoint(int i, float* p);

	void setPoint(int i, std::vector<float> p);

	void setCell(int cell, std::vector<int> vertices);

	void setClass(std::string newcls);

	int	 getCell(int cell, int* vertices);

	std::string getClass();



protected:

	std::vector<float> pointsX, pointsY, pointsZ;
	std::vector<int>	cells;
	std::string	cls;
};



