#include <DirectXMath.h>

#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

using namespace DirectX;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texcoords;
};

struct VertexTypeDef
{
	XMFLOAT3 position;
	XMFLOAT2 texcoords;
	XMFLOAT3 normal;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};
#endif