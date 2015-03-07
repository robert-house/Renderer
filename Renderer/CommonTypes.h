# pragma once

#include <DirectXMath.h>

using namespace DirectX;

// COMMON TYPES FOR EASY ACCESS


// Vertex Shader CBuffer
struct VS_CONSTANT_BUFFER_DATA
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 world;
	XMFLOAT4X4 texture;
};

struct CBUFFER_PER_OBJECT
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 texture;
};

struct CBUFFER_PER_FRAME
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
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
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};