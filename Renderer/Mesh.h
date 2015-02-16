#pragma once

#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include "VertexTypes.h"

using namespace std;

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Init(LPCWSTR fileName);
	vector <VertexTypeDef> GetVertexArray();
	void GetIndexArray(unsigned short *indices);
	bool Release();


private:
	bool LoadFromFile(LPCWSTR fileName);

	ModelType *pModel;
	int pVertexCount;
	int pIndexCount;
};