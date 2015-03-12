#pragma once

#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include "CommonTypes.h"
#include "SimpleMath.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class Model
{
public:
	struct TempVert
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 norm;
	};

public:
	Model();
	~Model();

	bool Init(LPCWSTR fileName);
	vector <VertexTypeDef> GetVertexArray();
	void GetIndexArray(unsigned short *indices);
	bool Release();

	unsigned int					getNumVerts();
	unsigned int					getNumIndex();
	VS_CBUFFER_PER_OBJECT				getCBuffer();


private:

	bool LoadFromFile(LPCWSTR fileName);
	void CalculateTangentBiNormal(TempVert vertex1, TempVert vertex2, TempVert vertex3,
		Vector3& tangent, Vector3& binormal);
	void CalculateNormal(Vector3, Vector3, Vector3&);
	void CalculateVectors();

	ModelType *pModel;
	vector<VertexTypeDef> _vertices;
	unsigned int _VertexCount;
	unsigned int _IndexCount;
	VS_CBUFFER_PER_OBJECT _cbuffer;
};