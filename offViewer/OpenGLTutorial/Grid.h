#pragma once

#include <vector>

using namespace std;




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

	void setPoint(int i, vector<float> p);

	void setCell(int cell, vector<int> vertices);

	int	 getCell(int cell, int* vertices);



protected:

	vector<float>		pointsX, pointsY, pointsZ;
	vector<int>			cells;
};



