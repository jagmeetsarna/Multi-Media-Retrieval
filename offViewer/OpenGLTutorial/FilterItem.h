#pragma once

#include <string>

struct FilterItem
{
	std::string cls;
	int numFaces;
	int numVertices;
	std::string typeOfFace;
	int minX, maxX, minY, MaxY, minZ, maxZ;

};