#ifndef CBUFFERS_H
#define CBUFFERS_H

#include <DirectXMath.h>

using namespace DirectX;

struct VS_DEFERRED_CBUFFER
{
	// Transformation Matrices
	XMFLOAT3 world;
	XMFLOAT3 proj;
	XMFLOAT3 view;
	XMFLOAT3 texture;
};

#endif