#pragma once

#include <vector>
#include "VectorAttributes.h"
#include "ScalarAttributes.h"
#include <stdio.h>
#include <string>
#include <Eigen/Dense>

using namespace std;


class Grid
{
public:
	Grid(int P, int C): scalars(P), pointNormals(P), faceNormals(C), pointsZ(P)															//Resize the array for the points and cells for the grid
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

	void setClass(string newcls);

	string getClass();

	int	 getCell(int cell, int* vertices);

	int findCell(float* p);

	void normalize();

	void computeFaceNormals();

	void computeVertexNormals();

	void computeCovarianceMatrix();

	void computeEigenvectors();

	VectorAttributes& getFaceNormals()
	{
		return faceNormals;
	}

	VectorAttributes& getPointNormals()
	{
		return pointNormals;
	}

	Eigen::Matrix3f& getCovarianceMatrix() {
		return covarianceMatrix;
	}

	vector<vector<float>> getEigenvectors() {
		vector<vector<float>> vectors;
		vectors.push_back(eigenVec1);
		vectors.push_back(eigenVec2);
		vectors.push_back(eigenVec3);

		return vectors;
	}


protected:

	ScalarAttributes	scalars;

	Eigen::Matrix3f		covarianceMatrix;

	vector<float>		eigenVec1, eigenVec2, eigenVec3;

	vector<float>		pointsX, pointsY, pointsZ;
	vector<int>			cells;
	VectorAttributes    pointNormals;
	VectorAttributes    faceNormals;
	std::string	cls;

};



