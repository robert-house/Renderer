#pragma once

#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include "CommonTypes.h"

using namespace std;

class Model
{
public:
	Model();
	~Model();

	bool Init(LPCWSTR fileName);
	vector <VertexTypeDef> GetVertexArray();
	void GetIndexArray(unsigned short *indices);
	bool Release();

	unsigned int					getNumVerts();
	unsigned int					getNumIndex();
	CBUFFER_PER_OBJECT				getCBuffer();


private:
	bool LoadFromFile(LPCWSTR fileName);

	ModelType *pModel;
	unsigned int _VertexCount;
	unsigned int _IndexCount;
	CBUFFER_PER_OBJECT _cbuffer;
};