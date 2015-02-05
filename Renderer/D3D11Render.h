//--------------------------------------------------------------------------------------
// File: D3D11Render.h
//
// This application displays a triangle rendered using Direct3D 11. (TEST BRANCH WORKS!)
//
// Concepts:
//     Creating a device and swap chain
//     Compiling and creating shaders
//     Setting up the render target and viewport
//     Setting up a simple vertex buffer and its input layout
//     Clearing the render target
//     Drawing a triangle
//     Presenting the rendered frame to the display
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

#ifndef	D3D11RENDER_H
#define D3D11RENDER_H

#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <DirectXColors.h>
#include <vector>

/* LOCAL COMPONENTS */
#include "InputAssembler.h"
#include "RenderTarget.h"

using namespace DirectX;
using namespace std;

// Vertex Shader CBuffer
struct VS_CONSTANT_BUFFER_DATA
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 texture;
};

class D3D11Render
{
	/* Globals */
	static const int	NUMTARGETS = 3;
	bool				g_windowed;
	bool				g_vsync;
	float				g_screenDepth;
	float				g_screenNear;
	float				g_aspectRatio;
	int					g_screenWidth;
	int					g_screenHeight;
	int					g_refreashRateNum = 60;
	int					g_refreashRateDen = 1;
	D3D_FEATURE_LEVEL	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	unsigned int					count = 0; // Can delete when I have my timer finished

	HWND							g_hwnd;
	ID3D11Device*					g_Device;
	vector <IDXGIAdapter*>			g_Adapters;
	ID3D11DeviceContext*			g_ImmContext;
	IDXGISwapChain*					g_SwapChain;
	ID3D11RenderTargetView*			g_RenderTargetView;
	D3D11_VIEWPORT					g_ViewPort;
	ID3D11Buffer*					g_ConstantBuffer;
	VS_CONSTANT_BUFFER_DATA			vs_ConstantBufferData;
	InputAssembler* ia;

	// RenderTarget
	RenderTarget *gBuffer;


	/* Function Prototypes */
public:
	D3D11Render();				// Constructor
	~D3D11Render();				// Destructor

	bool Init(float screenWidth, float screenHeight, HWND handle, bool vsync, bool window, 
		float depth, float snear);
	void Draw();
	bool ShutDown();
	void Update();

private:
	HRESULT DeviceSetup();
	HRESULT BackBufferSetup();
	void ConstantBufferSetup();

	/* Set Methods */
public:
	void SetCameraData(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix);


	/* Test Components */
public:
	
	void TestComponents();
	//vector <IDXGIAdapter*> D3D11Render::EnumerateAdapters();
	//void D3D11Render::GetDisplayModes();
	void CreateGBuffer();
	void RenderToTexture(ID3D11Device *device);

};

#endif D3D11RENDER_H