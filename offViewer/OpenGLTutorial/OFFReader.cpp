// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include "OFFReader.h"
#include "Grid.h"
#include "Renderer.h"
#include "FilterItem.h"
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


std::tuple<Grid*, FilterItem> openFile(string fileName)
{
	FilterItem fi;
	Grid* grid;

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

		fi.numVertices = stoi(stringVector[0]);
		fi.numFaces = stoi(stringVector[1]);

		int i, j;
		grid = new Grid(fi.numVertices, fi.numFaces);

		float mix = FLT_MAX, miy = FLT_MAX, miz = FLT_MAX;
		float max = FLT_MIN, may = FLT_MIN, maz = FLT_MIN;

		for (j = 0; j < fi.numVertices; j++)
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

			max = std::max(v.x, max);
			may = std::max(v.y, may);
			maz = std::max(v.z, maz);
			
			mix = std::min(v.x, mix);
			miy = std::min(v.y, miy);
			miz = std::min(v.z, miz);
		}

		fi.maxX = max;
		fi.maxY = may;
		fi.maxZ = maz;
		
		fi.minX = mix;
		fi.minY = miy;
		fi.minZ = miz;

		bool allTriangles = true, allQuads = true;

		for (j = 0; j < fi.numFaces; j++)
		{
			string line;
			getline(offFile, line);
			vector<string> vert;
			stringVector = split(line, ' ');
			Face face;
			int vertex_num = stoi(stringVector[0]);
			allTriangles &= vertex_num == 3;
			allQuads &= vertex_num == 4;
			for (int i = 0; i < vertex_num; i++) {
				face.verts.push_back(stoi(stringVector[i + 1]));
			}
			grid->setCell(j, face.verts);
		}

		if (allTriangles)
			fi.typeOfFace = "triangles";
		else if (allQuads)
			fi.typeOfFace = "quads";
		else
			fi.typeOfFace = "mix";

		return make_tuple(grid, fi);
	}


	else {
		cout << "FILE DOES NOT EXIST!" << endl;
		return make_tuple(grid, fi);
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

