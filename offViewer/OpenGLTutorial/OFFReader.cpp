// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "OFFReader.h"
#include "Grid.h"
#include "Renderer.h"
using namespace std;

typedef struct Vertex
{
	float x, y, z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face
{
	unsigned char nverts;    /* number of vertex indices in list */
	vector<int> verts;              /* vertex index list */
} Face;


Grid* openFile(string fileName) {

	int vertex_count, faces_count;

	if (file_exists) {
		ifstream offFile(fileName);
		string line;

		getline(offFile, line);
		if (line != "OFF") {
			cout << "NOT A .OFF FILE!" << endl;
		}

		string counterLine;
		getline(offFile, counterLine);
		vector<string> stringVector;
		stringVector = split(counterLine, ' ');

		vertex_count = stoi(stringVector[0]);
		faces_count = stoi(stringVector[1]);

		int i, j;
		Grid* grid = new Grid(vertex_count, faces_count);

		for (j = 0; j < vertex_count; j++)
		{
			getline(offFile, line);
			vector<string> vert;
			stringVector = split(line, ' ');
			Vertex v;
			vector<float> V;
			v.x = stof(stringVector[0]);
			v.y = stof(stringVector[1]);
			v.z = stof(stringVector[2]);
			V.push_back(v.x);
			V.push_back(v.y);
			V.push_back(v.z);
			grid->setPoint(j, V);
		}

		for (j = 0; j < faces_count; j++)
		{
			string line;
			getline(offFile, line);
			vector<string> vert;
			stringVector = split(line, ' ');
			Face face;
			int vertex_num = stoi(stringVector[0]);
			for (int i = 0; i < vertex_num; i++) {
				face.verts.push_back(stoi(stringVector[i + 1]));
			}
			grid->setCell(j, face.verts);
		}

		return grid;


	}


	else {
		cout << "FILE DOES NOT EXIST!" << endl;
		return NULL;
	}

}

bool file_exists(const char* fileName) {
	ifstream infile(fileName);
	return infile.good();
}

vector<string> split(string str, char delimiter) {
	stringstream ss(str);
	vector<string> stringVec;
	string stringPiece;

	while (getline(ss, stringPiece, delimiter)) {
		stringVec.push_back(stringPiece);
	}
	return stringVec;
}

