#include "D3D11Render.h"

//================= Constructor =========================//
// Sets some of the pointer values to nullptr.
D3D11Render::D3D11Render()
{
	_hwnd = nullptr;
	_device = nullptr;
	_immContext = nullptr;
	_swapChain = nullptr;
	_renderTargetView = nullptr;
	_constantBuffer[0] = nullptr;
	_constantBuffer[1] = nullptr;
	_inputAssembler = nullptr;
	gBuffer = nullptr;
	_psCBuffer = nullptr;
}

//================= Destructor =========================//
// Suppose to break down this object on shutdown. Not
// implemented yet.
D3D11Render::~D3D11Render() {}


#pragma region Setup

//=================[ Init ]============================
// Init_inputAssemblerlizes this Direct3D object
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// screenWidth		- Width of the screen in pixels
// screenHeight		- Height of the screen in pixels
// handle			- Window to output to
// vsync			- VSync Enabled/Disabled
// isWindowed		- windowed/fullscreen mode
// farPlane			- Farplane value
// nearPlane		- clipplane value
//===================================================
bool D3D11Render::Init(float screenWidth, float screenHeight, HWND handle, bool vsync,
	bool windowed, float farPlane, float nearPlane)
{
	// TEST
	_constantPerFrame.globalLight = XMFLOAT4(-2.0f, 1.0f, 2.0f, 0.0f);
	_constantPerFrame.globalColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Assign Globals
	_screenWidth		= screenWidth;
	_screenHeight		= screenHeight;
	_aspectRatio		= _screenWidth / _screenHeight;
	_hwnd				= handle;
	_vsync				= vsync;
	_isWindowed			= windowed;
	_screenDepth		= farPlane;
	_screenNear			= nearPlane;

	// Execute helper functions
	DeviceSetup();
	BackBufferSetup();
	ConstantBufferSetup();

	// Init Input Assembler
	_inputAssembler = new InputAssembler;
	_inputAssembler->Init(_device, _immContext);

	/* I have to rethink how these components are going to init.
	There must be a loop that precedes the rendering phase where
	all data is loaded from sysram->vram and the ok is given to
	render the frame */

	// Set number of render targets in the gBuffer and
	// then create it.
	CreateGBuffer(3);

	/* Set ViewPort */
	_viewPort.TopLeftX	= 0.0f;				// X Origin
	_viewPort.TopLeftY	= 0.0f;				// Y Origin
	_viewPort.Width		= _screenWidth;		// Width
	_viewPort.Height	= _screenHeight;	// Height
	_viewPort.MinDepth	= 0.0f;				// Normalized Depth Min
	_viewPort.MaxDepth	= 1.0f;				// Normalized Depth Max

	// Set Sampler state
	ID3D11SamplerState *_aniso;
	D3D11_SAMPLER_DESC anisoDesc;

	ZeroMemory(&anisoDesc, sizeof(anisoDesc));

	anisoDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	anisoDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	anisoDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	anisoDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	anisoDesc.MipLODBias = 0;
	anisoDesc.MaxAnisotropy = 16;
	anisoDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//anisotropicDesc.BorderColor = 0.0f;
	anisoDesc.MinLOD = 0;
	anisoDesc.MaxLOD = 0;

	_device->CreateSamplerState(&anisoDesc, &_aniso);
	_immContext->PSSetSamplers(0, 1, &_aniso);

	return true;
}

//================= Device Setup ============================
// Init_inputAssemblerlize the 3D hardware and store pointer to the hardware
// locally.
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
HRESULT D3D11Render::DeviceSetup()
{
	/* Set up Device and Swapchain. First we need to fill out a swap
	chain description */

	// Fill out SwapChainDescription parameters
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// Clear memory
	sd.BufferCount = 1;									// Backbuffer count. Not double buffered
	sd.BufferDesc.Width = _screenWidth;					// Width of the backbuffer
	sd.BufferDesc.Height = _screenHeight;				// Height of the backbuffer
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Format, RGBA8
	sd.BufferDesc.RefreshRate.Numerator = 1;			// Refreshrate numerator
	sd.BufferDesc.RefreshRate.Denominator = 0;			// SHOULD NOT BE ZERO
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Usage of buffer - render target output
	sd.OutputWindow = _hwnd;							// Window to output to
	sd.Windowed = _isWindowed;							// Windowed || Fullscreen

	// MultiSampling crap. Implement later if needed
	// NOT IMPLEMENTED
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	/*
	==============================
	INSERT FEATURE LEVEL CODE HERE 
	==============================
	*/


	// Init_inputAssemblerlize device and swap chain
	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL featurelevel; // Feature level in the interface


	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						// Adapter - We have 1 so send nullptr
		D3D_DRIVER_TYPE_HARDWARE,		// Hardware || Debug || Software Emulation
		NULL,							// HMODULE - ??
		0,								// Flags - Keep at 0
		&_featureLevel,					// Feature Level - D3D11.0
		1,								// Num feature levels supported
		D3D11_SDK_VERSION,				// SDK Version
		&sd,							// Swapchain Description
		&_swapChain,					// Dereferenced pointer to swapchain
		&_device,						// Dereferenced pointer to device
		&featurelevel,					// DO NOT KNOW
		&_immContext)))					// Immed_inputAssemblerte Device Context
	{
		return hr;
	}

	return hr;
}

//================= BackBufferSetup ============================
// Init_inputAssemblerlize the backbuffer texture and bind it to 
// a render target view.
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==============================================================
HRESULT D3D11Render::BackBufferSetup()
{
	HRESULT hr = S_OK;

	// Backbuffer pointer to temp store the object while
	// we construct this.
	ID3D11Buffer *backbuffer;

	// Bind backbuffer to swapchain
	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&backbuffer)))
	{
		return hr;
	}

	// Create and bind Render Target View for the back buffer
	if (FAILED(_device->CreateRenderTargetView(
		backbuffer,			  // Render target to use 
		nullptr,			  // Render target description to use
		&_renderTargetView))) // Render target view to use
	{
		return hr;
	}

	// Delete backbuffer pointer as it is no longer needed
	backbuffer->Release();
	backbuffer = nullptr;

	return hr;
}

//================= [ConstantBufferSetup] ============================
// Init_inputAssemblerlize and bind the constant buffer to the immed_inputAssemblerte device
// context.
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
void D3D11Render::ConstantBufferSetup()
{	
	// Create Constant Buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;
	bd.ByteWidth			= sizeof(VS_CBUFFER_PER_FRAME);
	bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags		= 0;// D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags			= 0;
	bd.StructureByteStride	= 0;

	_device->CreateBuffer(
		&bd,				// Buffer Description
		nullptr,			// Subresource data = single mipmap-level surface
		&_constantBuffer[0]	// pointer to bind to
		);

	bd.ByteWidth = sizeof(VS_CBUFFER_PER_OBJECT);

	_device->CreateBuffer(
		&bd,				// Buffer Description
		nullptr,			// Subresource data = single mipmap-level surface
		&_constantBuffer[1]	// pointer to bind to
		);

	//TEST STUFF
	bd.ByteWidth = sizeof(PS_CBUFFER_PER_FRAME);
	_device->CreateBuffer(&bd, nullptr, &_psCBuffer);
}

#pragma endregion

#pragma region Mutators

//================= SetCameraData ===================================
// Bind camera data from the model to the constant buffer
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// viewMatrix		- 4X4 matrix containing the view transform
// projMatrix		- 4X4 matrix containing the projection transform
//===================================================================
void D3D11Render::SetCameraData(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix, XMFLOAT3 viewVec)
{
	_vsPerFrame.view = viewMatrix;
	_vsPerFrame.proj = projMatrix;

	// Pixel shader stuff
	_constantPerFrame.viewVector = XMFLOAT4(viewVec.x, viewVec.y, viewVec.z, 1.0f);
}

#pragma endregion

//================= [Draw] =========================
// Draw objects to the render targets
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
void D3D11Render::RenderToBackBuffer()
{	
	// Prepare for RENDERING!
	// Set the viewport for this pass
	_immContext->RSSetViewports(1, &_viewPort);

	// Render to gBuffer
	//RenderToTexture();

	// Set render target back to the backbuffer
	_immContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	// Bind shaders to the pipeline
	_inputAssembler->BindShaders();

	// Draw using binded shaders
	_immContext->DrawIndexed(3, 0, 0);	// Should replace with using an index to draw and pull index count from the InputAssembler class
	
	_swapChain->Present(1, 0); // Set to 0 if no vsync
}

//=================[RenderToTexture]=============================
// Render geometry to seperate rendertargets
// --------------	---------------------------------------------
// |   Params   |	|				Description				    |
// --------------	---------------------------------------------
//===============================================================
void D3D11Render::RenderToTexture(int start, int offset, int vert)
{
	// Store render target views in a temp var so we can
	// more easily clear the render target.
	ID3D11RenderTargetView **tempRTV = gBuffer->GetRenderTargetView();

	// Store number of render targets in the gBuffer
	unsigned int numRT = gBuffer->GetNumRT();

	//Set RTV
	_immContext->OMSetRenderTargets(numRT, tempRTV, nullptr);

	// Bind Defered Shaders
	_inputAssembler->BindDeferredShaders();

	//Render to Texture
	_immContext->DrawIndexed(36, 0, 0);
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
TEST STUFF
----------------------------------------------*/
void D3D11Render::TestComponents()
{
	// All this stuff works, move it into the class proper
	// probably ComponentSetup()

	
}


//=================[CreateGBuffer]===============================
// Create GBuffer containing n render targets.
// --------------	---------------------------------------------
// |   Params   |	|				Description				    |
// --------------	---------------------------------------------
// numOfRT			- The number of render targets to create
//===============================================================
void D3D11Render::CreateGBuffer(int numOfRT)
{
	gBuffer = new RenderTarget();
	gBuffer->Init(_device, _screenWidth, _screenHeight, numOfRT);

	// Set deferred resource
	ID3D11ShaderResourceView **tempsrv;
	tempsrv = gBuffer->GetRenderTargetResource();
	for (int i = 0; i < numOfRT; i++)
	{
		_inputAssembler->SetDeferredResource(tempsrv[i]);
	}
}

void D3D11Render::Update()
{
	// Set Texture Transform Matrix
	XMStoreFloat4x4(
		&_vsPerObject.texture,
		XMMatrixIdentity());

	// Copy the updated constant buffer from system memory to video memory.
	_immContext->UpdateSubresource(
		_constantBuffer[0],
		0,      // update the 0th subresource
		NULL,   // use the whole destination
		&_vsPerFrame,
		0,      // default pitch
		0       // default pitch
		);

	_immContext->UpdateSubresource(
		_constantBuffer[1],
		0,      // update the 0th subresource
		NULL,   // use the whole destination
		&_vsPerObject,
		0,      // default pitch
		0       // default pitch
		);

	_immContext->UpdateSubresource(
		_psCBuffer,
		0,      // update the 0th subresource
		NULL,   // use the whole destination
		&_constantPerFrame,
		0,      // default pitch
		0       // default pitch
		);

	_immContext->VSSetConstantBuffers(0, 1, &_constantBuffer[0]);
	_immContext->VSSetConstantBuffers(1, 1, &_constantBuffer[1]);
	_immContext->PSSetConstantBuffers(0, 1, &_psCBuffer);
}

InputAssembler* D3D11Render::RegisterIA()
{
	return _inputAssembler;
}

void D3D11Render::setWorldMatrix(XMFLOAT4X4 wm)
{
	_vsPerObject.world = wm;
}

void D3D11Render::ClearRTs()
{
	// Clear backbuffer
	_immContext->ClearRenderTargetView(_renderTargetView, DirectX::Colors::CornflowerBlue);

	// Store render target views in a temp var so we can
	// more easily clear the render target.
	ID3D11RenderTargetView **tempRTV = gBuffer->GetRenderTargetView();

	// Store number of render targets in the gBuffer
	unsigned int numRT = gBuffer->GetNumRT();

	//Clear MRT Textures in the gBuffer
	for (int i = 0; i < numRT; i++)
	{
		_immContext->ClearRenderTargetView(tempRTV[i], DirectX::Colors::SkyBlue);
	}
}