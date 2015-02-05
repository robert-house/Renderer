#include "D3D11Render.h"

/* Functions */
D3D11Render::D3D11Render()
{
	g_hwnd = NULL;
	g_Device = NULL;
	g_ImmContext = NULL;
	g_SwapChain = NULL;
	g_RenderTargetView = NULL;
}
bool D3D11Render::Init(float screenWidth, float screenHeight, HWND handle, bool vsync,
	bool window, float depth, float snear)
{
	// Assign Globals
	g_screenWidth = screenWidth;
	g_screenHeight = screenHeight;
	g_aspectRatio = g_screenWidth / g_screenHeight;
	g_hwnd = handle;
	g_vsync = vsync;
	g_windowed = window;
	g_screenDepth = depth;
	g_screenNear = snear;

	// Execute helper functions
	DeviceSetup();
	BackBufferSetup();
	ConstantBufferSetup();
	TestComponents();
	CreateGBuffer();

	/* Set ViewPort */
	g_ViewPort.TopLeftX = 0.0f;
	g_ViewPort.TopLeftY = 0.0f;
	g_ViewPort.Width = g_screenWidth;
	g_ViewPort.Height = g_screenHeight;
	g_ViewPort.MinDepth = 0.0f;
	g_ViewPort.MaxDepth = 1.0f;

	return true;
}
void D3D11Render::Draw()
{
	// Prepare for RENDERING!
	g_ImmContext->RSSetViewports(1, &g_ViewPort);
	RenderToTexture(g_Device);
	g_ImmContext->OMSetRenderTargets(1, &g_RenderTargetView, NULL);

#pragma region Stupid BackBuffer Clearing
	g_ImmContext->ClearRenderTargetView(g_RenderTargetView, DirectX::Colors::CornflowerBlue);
	count++;
	Update();
#pragma endregion

	// All other Rendering goes here!
	ia->RenderShaders();
	g_ImmContext->DrawIndexed(36, 0, 0);	// Should replace with using an index to draw and pull index count from the InputAssembler class
	g_SwapChain->Present(1, 0); // Set to 0 if no vsync
}

HRESULT D3D11Render::DeviceSetup()
{
	/* Set up Device and Swapchain. First we need to fill out a swap
	chain description */

	// SwapChain Description
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd)); // Clear memory
	sd.BufferCount = 1;
	sd.BufferDesc.Width = g_screenWidth;
	sd.BufferDesc.Height = g_screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hwnd;
	sd.Windowed = g_windowed;

	// MultiSampling crap. Implement later if needed
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	// HardCoded Feature level. Code to enum device and pull
	// it automatically.


	// Create Device and SwapChain
	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL featurelevel;

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&g_featureLevel,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&g_SwapChain,
		&g_Device,
		&featurelevel,
		&g_ImmContext)))
	{
		return hr;
	}

	return hr;
}
HRESULT D3D11Render::BackBufferSetup()
{
	HRESULT hr = S_OK;
	ID3D11Buffer *backbuffer;

	// Bind backbuffer to swapchain
	if (FAILED(g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&backbuffer)))
	{
		return hr;
	}

	// Create Render Target View
	if (FAILED(g_Device->CreateRenderTargetView(backbuffer, NULL,
		&g_RenderTargetView)))
	{
		return hr;
	}

	// Delete backbuffer pointer as it is no longer needed
	backbuffer->Release();
	backbuffer = NULL;

	return hr;
}
void D3D11Render::ConstantBufferSetup()
{
	// Create Constant Buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vs_ConstantBufferData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	g_Device->CreateBuffer(
		&bd,
		nullptr,
		&g_ConstantBuffer
		);

	// Bind Constant Buffer to device context
	g_ImmContext->VSSetConstantBuffers(0, 1, &g_ConstantBuffer);
}
void D3D11Render::SetCameraData(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix)
{
	vs_ConstantBufferData.view = viewMatrix;
	vs_ConstantBufferData.proj = projMatrix;
}
bool D3D11Render::ShutDown()
{
	g_hwnd = NULL;

	g_RenderTargetView->Release();
	g_RenderTargetView = NULL;

	g_Device->Release();
	g_Device = NULL;

	g_ImmContext->Release();
	g_ImmContext = NULL;

	g_SwapChain->Release();
	g_SwapChain = NULL;

	return true;
}

/* -------------------------------------------
TEST SHIT
----------------------------------------------*/
void D3D11Render::TestComponents()
{
	// All this shit works, move it into the class proper
	// probably ComponentSetup()

	ia = new InputAssembler;

	//INIT COMPONENTS
	ia->Init(g_Device, g_ImmContext);

	/* I have to rethink how these components are going to initialize.
	There must be a loop that precedes the rendering phase where
	all data is loaded from sysram->vram and the ok is given to
	render the frame */
}

void D3D11Render::CreateGBuffer()
{
	gBuffer = new RenderTarget();
	gBuffer->Init(g_Device, g_screenWidth, g_screenHeight, NUMTARGETS);

	// Set deferred resource
	ID3D11ShaderResourceView **tempsrv;
	tempsrv = gBuffer->GetRenderTargetResource();
	for (int i = 0; i < NUMTARGETS; i++)
	{
		ia->SetDeferredResource(tempsrv[i]);
	}
}

void D3D11Render::RenderToTexture(ID3D11Device *device)
{
	ID3D11DeviceContext *context;
	float Colors[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	device->GetImmediateContext(&context);

	unsigned int numRT = 0;
	numRT = gBuffer->GetNumRT();
	ID3D11RenderTargetView **tempRTV;
	tempRTV = gBuffer->GetRenderTargetView();


	//Clear MRT Textures
	for (int i = 0; i < numRT; i++)
	{
		context->ClearRenderTargetView(tempRTV[i], Colors);
	}

	//Set RTV
	context->OMSetRenderTargets(NUMTARGETS, tempRTV, NULL);

	// Set Defered Shaders
	ia->RenderDeferred();

	//Render to Texture
	g_ImmContext->DrawIndexed(36, 0, 0);
}

void D3D11Render::Update()
{
	// World Matrix (For Now)
	XMStoreFloat4x4(
		&vs_ConstantBufferData.world,
		XMMatrixTranspose(XMMatrixRotationY(count*XM_PI / 180.f))
		);

	// Set Texture Transform Matrix
	XMStoreFloat4x4(
		&vs_ConstantBufferData.texture,
		XMMatrixIdentity());

	// Copy the updated constant buffer from system memory to video memory.
	g_ImmContext->UpdateSubresource(
		g_ConstantBuffer,
		0,      // update the 0th subresource
		NULL,   // use the whole destination
		&vs_ConstantBufferData,
		0,      // default pitch
		0       // default pitch
		);
}