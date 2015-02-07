#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

/* TAGGED FOR STYLE CLEAN UP */

class RenderTarget
{
// This could backfire in my face, have to be careful
	static const unsigned int MAX_RENDER_TARGETS = 12;

public:
	RenderTarget();
	~RenderTarget();

	bool Init(ID3D11Device *device, int width, int height, unsigned int numRT);
	bool CreateRenderTargets(ID3D11Device *device, int width, int height);
	bool Release();

	ID3D11RenderTargetView** GetRenderTargetView();
	ID3D11ShaderResourceView** GetRenderTargetResource();
	unsigned int GetNumRT();

private:
	unsigned int pNumRT;
	ID3D11Texture2D *pRenderTarget[MAX_RENDER_TARGETS];
	ID3D11RenderTargetView *pRTV[MAX_RENDER_TARGETS];
	ID3D11ShaderResourceView *pSRV[MAX_RENDER_TARGETS];
};

#endif