#include "D3D11Render.h"

/* Functions */
D3D11Render::D3D11Render()
{
	_hwnd = NULL;
	_device = NULL;
	_immContext = NULL;
	_swapChain = NULL;
	_renderTargetView = NULL;
}

D3D11Render::~D3D11Render() {}

#pragma region Setup

bool D3D11Render::Init(float screenWidth, float screenHeight, HWND handle, bool vsync,
	bool windowed, float depth, float snear)
{
	// Assign Globals
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_aspectRatio = _screenWidth / _screenHeight;
	_hwnd = handle;
	_vsync = vsync;
	_isWindowed = windowed;
	_screenDepth = depth;
	_screenNear = snear;

	// Execute helper functions
	DeviceSetup();
	BackBufferSetup();
	ConstantBufferSetup();
	TestComponents();
	CreateGBuffer();

	/* Set ViewPort */
	_viewPort.TopLeftX = 0.0f;
	_viewPort.TopLeftY = 0.0f;
	_viewPort.Width = _screenWidth;
	_viewPort.Height = _screenHeight;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;

	return true;
}

HRESULT D3D11Render::DeviceSetup()
{
	/* Set up Device and Swapchain. First we need to fill out a swap
	chain description */

	// SwapChain Description
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd)); // Clear memory
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _screenWidth;
	sd.BufferDesc.Height = _screenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.Windowed = _isWindowed;

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
		&_featureLevel,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&_swapChain,
		&_device,
		&featurelevel,
		&_immContext)))
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
	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&backbuffer)))
	{
		return hr;
	}

	// Create Render Target View
	if (FAILED(_device->CreateRenderTargetView(backbuffer, NULL,
		&_renderTargetView)))
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
	bd.ByteWidth = sizeof(_constantBufferData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	_device->CreateBuffer(
		&bd,
		nullptr,
		&_constantBuffer
		);

	// Bind Constant Buffer to device context
	_immContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
}

#pragma endregion

#pragma region Mutators

void D3D11Render::SetCameraData(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	_constantBufferData.view = viewMatrix;
	_constantBufferData.proj = projMatrix;
}

#pragma endregion

void D3D11Render::Draw()
{
	// Prepare for RENDERING!
	// Set the viewport for this pass
	_immContext->RSSetViewports(1, &_viewPort);

	// Render to gBuffer
	RenderToTexture(_device);

	// Set render target back to the backbuffer
	_immContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	// Clear backbuffer
	_immContext->ClearRenderTargetView(_renderTargetView, DirectX::Colors::CornflowerBlue);
	
	// Increment counter then Update the scene
	// I think this should go after the drawing to the backbuffer
	count++;
	Update();

	// Bind shaders to the pipeline
	ia->BindShaders();

	// Draw using binded shaders
	_immContext->DrawIndexed(36, 0, 0);	// Should replace with using an index to draw and pull index count from the InputAssembler class
	_swapChain->Present(1, 0); // Set to 0 if no vsync
}

bool D3D11Render::ShutDown()
{
	_hwnd = NULL;

	_renderTargetView->Release();
	_renderTargetView = NULL;

	_device->Release();
	_device = NULL;

	_immContext->Release();
	_immContext = NULL;

	_swapChain->Release();
	_swapChain = NULL;

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
	ia->Init(_device, _immContext);

	/* I have to rethink how these components are going to initialize.
	There must be a loop that precedes the rendering phase where
	all data is loaded from sysram->vram and the ok is given to
	render the frame */
}

void D3D11Render::CreateGBuffer()
{
	gBuffer = new RenderTarget();
	gBuffer->Init(_device, _screenWidth, _screenHeight, NUMTARGETS);

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
	ia->BindDeferredShaders();

	//Render to Texture
	_immContext->DrawIndexed(36, 0, 0);
}

void D3D11Render::Update()
{
	// World Matrix (For Now)
	XMStoreFloat4x4(
		&_constantBufferData.world,
		XMMatrixTranspose(XMMatrixRotationY(count*XM_PI / 180.f))
		);

	// Set Texture Transform Matrix
	XMStoreFloat4x4(
		&_constantBufferData.texture,
		XMMatrixIdentity());

	// Copy the updated constant buffer from system memory to video memory.
	_immContext->UpdateSubresource(
		_constantBuffer,
		0,      // update the 0th subresource
		NULL,   // use the whole destination
		&_constantBufferData,
		0,      // default pitch
		0       // default pitch
		);
}