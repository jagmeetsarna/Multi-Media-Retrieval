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
	ifstream first_file(fileName);

	if (first_file) {
		ifstream first_file(fileName);
		string line;

		/*while (getline(first_file, line)) {
			cout << line << endl;
		}*/

		if (first_file.good()) {
			if (getline(first_file, line));
			cout << line << endl;
		}
		if (line == "ply") {										//If the file is in the .ply format, we use a differnet function
			
			readPlyFile(fileName);

		}
		first_file.close();

		ifstream offFile(fileName + ".off");
		getline(offFile, line);
		line = "";

		string counterLine;
		getline(offFile, counterLine);
		vector<string> string_vector;
		string_vector = split(counterLine, ' ');

		vertex_count = stoi(string_vector[0]);
		faces_count = stoi(string_vector[1]);

		int i, j;
		Grid* grid = new Grid(vertex_count, faces_count);

		for (j = 0; j < vertex_count; j++)
		{

			string line;
			getline(offFile, line);
			vector<string> vert;
			string_vector = split(line, ' ');
			Vertex v;
			vector<float> V;
			v.x = stof(string_vector[0]);
			v.y = stof(string_vector[1]);
			v.z = stof(string_vector[2]);
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
			string_vector = split(line, ' ');
			Face face;
			int vertex_num = stoi(string_vector[0]);
			for (int i = 0; i < vertex_num; i++) {
				face.verts.push_back(stoi(string_vector[i + 1]));
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

void readPlyFile(string fileName) {

	int vertex_count = 0;
	int faces_count = 0;

	ifstream ply_file(fileName);
	ofstream off_file(fileName + ".off");

	string ply_line;

	while (getline(ply_file, ply_line)) {										//Go through the header line by line

		vector<string> string_vector;
		string_vector = split(ply_line, ' ');

		if (string_vector[0] == "end_header") break;
		else if (string_vector[0] == "element") {
			if (string_vector[1] == "vertex") {
				vertex_count = stoi(string_vector[2]);
			}
			else if (string_vector[1] == "face") {
				faces_count = stoi(string_vector[2]);
			}
		}
	
	}
	off_file << "OFF" << endl;
	off_file << to_string(vertex_count) + " " + to_string(faces_count) << endl;
	while (getline(ply_file, ply_line)) {
		off_file << ply_line << endl;
	}

	off_file.close();
	ply_file.close();




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

