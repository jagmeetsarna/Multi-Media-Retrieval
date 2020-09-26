#include "Grid.h"
#include <iostream>
#include <algorithm>

using namespace std;



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

	/*int len = sizeof(vertices);

	for (int i = 0; i < len; i++) {
		cout << vertices[i] << endl;
		vertices[i] = cells[len * cell + i];
	}*/

	return 3;
}

void Grid::setCell(int cell, vector<int> vertices)
{
	/*cells[3 * cell] = vertices[0];
	cells[3 * cell + 1] = vertices[1];
	cells[3 * cell + 2] = vertices[2];*/

	int len = vertices.size();
	for (int i = 0; i < len; i++) {
		cells[3 * cell + i] = vertices[i];
	}

}

void Grid::normalize()												//Normalize the grid in the [-1,1] cube
{
	float minX = 1.0e6, minY = 1.0e6, minZ = 1.0e6;
	float maxX = -1.0e6, maxY = -1.0e6, maxZ = -1.0e6;

	for (int i = 0; i < numPoints(); ++i)							//1. Determine the bounding-box of all points in the grid
	{
		float p[3];
		getPoint(i, p);
		minX = min(p[0], minX); maxX = max(p[0], maxX);
		minY = min(p[1], minY); maxY = max(p[1], maxY);
		minZ = min(p[2], minZ); maxZ = max(p[2], maxZ);
	}

	float sizeX = maxX - minX;										//2. Compute a single scaling factor that best fits the grid
	sizeX = (sizeX) ? 1 / sizeX : 1;								//   in the [-1,1] cube. Using a single factor for x,y, and z
	float sizeY = maxY - minY;										//   ensures that the object is scaled while keeping its
	sizeY = (sizeY) ? 1 / sizeY : 1;								//   aspect ratio.
	float sizeZ = maxZ - minZ;
	sizeZ = (sizeZ) ? 1 / sizeZ : 1;

	float scale = min(sizeX, min(sizeY, sizeZ));

	for (int i = 0; i < numPoints(); ++i)							//3. Use the scaling factor computed above to scale all grid
	{																//   points in the [-1,1] cube
		float p[3];
		getPoint(i, p);

		p[0] = 2 * ((p[0] - minX) * scale - 0.5);
		p[1] = 2 * ((p[1] - minY) * scale - 0.5);
		p[2] = 2 * ((p[2] - minZ) * scale - 0.5);

		vector<float> v;
		v.push_back(p[0]);
		v.push_back(p[1]);
		v.push_back(p[2]);
		setPoint(i, v);
	}
}

Point3d cellNormal(int size, Point3d* p)						//Compute the normal of a cell whose three vertices are given in p[]
{
	Point3d edge1 = p[1] - p[0];								//We assume that the cell is a triangle. Then, the normal is the
	Point3d edge2 = p[2] - p[1];								//normalized (unit length) value of the cross-product of two cell edges.
	Point3d normal = edge1.cross(edge2);
	normal.normalize();
	return normal;
}

void Grid::computeFaceNormals()								//Compute face normals for the grid. For this, we use the cellNormal()
{															//function presented above, for all grid triangles.
	for (int i = 0; i < numCells(); ++i)
	{
		int cell[10];
		int size = getCell(i, cell);

		Point3d points[10];
		for (int j = 0; j < size; ++j)
		{
			float p[3];
			getPoint(cell[j], p);
			points[j] = Point3d(p);
		}

		Point3d face_normal = cellNormal(size, points);

		faceNormals.setC0Vector(i, face_normal.data);
	}
}

void Grid::computeVertexNormals()							//Compute vertex normals for the grid. For this, we add, to each vertex,
{															//the normals of all cells that use that vertex. Next, we normalize the result.
	for (int i = 0; i < numCells(); ++i)
	{
		int cell[10];
		int size = getCell(i, cell);

		Point3d face_normal = faceNormals.getC0Vector(i);

		for (int j = 0; j < size; ++j)
		{
			Point3d point_normal = pointNormals.getC0Vector(cell[j]);

			point_normal += face_normal;

			pointNormals.setC0Vector(cell[j], point_normal.data);
		}
	}

	for (int i = 0; i < numPoints(); ++i)
	{
		Point3d point_normal = pointNormals.getC0Vector(i);
		point_normal.normalize();
		pointNormals.setC0Vector(i, point_normal.data);
	}
}

void Grid::computeCovarianceMatrix() {
	double means[3] = { 0, 0, 0 };
	vector<vector<float>> covariance;
	vector<vector<float>> points;

	for (int i = 0; i < 3; i++) {
		vector<float> entry;
		for (int j = 0; j < 3; j++) {
			entry.push_back(0.0f);
		}
		covariance.push_back(entry);
	}

	for (int i = 0; i < pointsX.size(); i++) {
		means[0] += pointsX[i];
		means[1] += pointsY[i];
		means[2] += pointsZ[i];
		vector<float> point;

		point.push_back(pointsX[i]);
		point.push_back(pointsY[i]);
		point.push_back(pointsZ[i]);
		points.push_back(point);
	}
		
	means[0] /= points.size(), means[1] /= points.size(), means[2] /= points.size();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			covariance[i][j] = 0.0f;
			for (int k = 0; k < points.size(); k++)
				covariance[i][j] += (means[i] - points[k][i]) *
				(means[j] - points[k][j]);
			covariance[i][j] /= points.size() - 1;
			//cout << covariance[i][j];
			//cout << ";";
		}
		//cout << endl;
	}
	covarianceMatrix << covariance[0][0], covariance[0][1], covariance[0][2],
		covariance[1][0], covariance[1][1], covariance[1][2],
		covariance[2][0], covariance[2][1], covariance[2][2];
	cout << covarianceMatrix << endl;
}

void Grid::computeEigenvectors() {

	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eig(covarianceMatrix);
	cout << "Eigenvalues: ";
	cout << eig.eigenvalues() << endl;
	cout << "Eigenvectors: ";
	cout << eig.eigenvectors() << endl << endl;
	cout << "1";
	cout << eig.eigenvectors()[0] << endl << endl;
	cout << "2";
	cout << eig.eigenvectors()[1] << endl << endl;
	cout << "3";
	cout << eig.eigenvectors()[2] << endl << endl;

}


string Grid::getClass()
{
	return cls;
}

void Grid::setClass(string newcls)
{
	cls = newcls;
}

