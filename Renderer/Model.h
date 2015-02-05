#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include "VertexTypes.h"

class Model
{
public:
	Model();
	~Model();

	bool Init(LPCWSTR fileName);
	std::vector <VertexTypeDef> GetVertexArray();
	void GetIndexArray(unsigned short *indices);
	bool Release();


private:
	bool Load(LPCWSTR fileName);

	ModelType *pModel;
	int pVertexCount;
	int pIndexCount;

};

#endif