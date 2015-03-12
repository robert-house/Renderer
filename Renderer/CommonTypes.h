# pragma once

#include <DirectXMath.h>

using namespace DirectX;

// COMMON TYPES FOR EASY ACCESS


// Vertex Shader CBuffer
struct VS_CONSTANT_BUFFER_DATA
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 texture;
};

struct VS_CBUFFER_PER_OBJECT
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 texture;
};

struct VS_CBUFFER_PER_FRAME
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
};

struct PS_CBUFFER_PER_FRAME
{
	XMFLOAT4 viewVector;
	XMFLOAT4 globalLight;
	XMFLOAT4 globalColor;
};

struct CBUFFER_PER_SCENE
{

};

/////////////////////////
// COMMON VERTEX TYPES //
/////////////////////////
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
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};