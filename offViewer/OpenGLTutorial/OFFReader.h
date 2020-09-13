#pragma once
#include <sstream>
#include <string>
#include <vector>
#include "Grid.h"
using namespace std;

Grid* openFile(string filename);								//Open the .off file and return the created grid
bool file_exists(const char* fileName);								//Check if the file exists
vector<string> split(string str, char delimiter);					//Split the lines of the file on the delimiter, returning a vector with the different string elements
