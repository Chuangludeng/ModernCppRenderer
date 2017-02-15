#include "model.h"

void ModelLoader::load(std::string file, Model &model)
{
	char lineHeader[256];
	FILE * _file = fopen(file.c_str(), "r");
	// read the first word of the line
	int vertexIndex1, vertexIndex2, vertexIndex3;
	vector_t vertice = { 0,0,0,1 };
	int res = fscanf(_file, "%s", lineHeader);
	while (res != EOF)
	{
		if (strcmp(lineHeader, "v") == 0) {
			fscanf(_file, "%f %f %f\n", &vertice.x, &vertice.y, &vertice.z);
			model.vertices.push_back(vertice);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			fscanf(_file, "%d %d %d\n", &vertexIndex1, &vertexIndex2, &vertexIndex3);
			model.indices.push_back(vertexIndex1);
			model.indices.push_back(vertexIndex2);
			model.indices.push_back(vertexIndex3);
		}
		res = fscanf(_file, "%s", lineHeader);
	}
	fclose(_file);
}

