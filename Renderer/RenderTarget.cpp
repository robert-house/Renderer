#include "RenderTarget.h"

RenderTarget::RenderTarget() {}
RenderTarget::~RenderTarget() {}


//================= [Init] =================================
// Initialize Render Target object
//
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// device			- Pointer to the current graphics device
// width			- Horizontal resolution
// height			- Vertical resolution
// numRT			- Number of render targets to create
//==========================================================
bool RenderTarget::Init(ID3D11Device *device, int width, int height, unsigned int numRT)
{
	// Set up this object
	_numRT = numRT;
	CreateRenderTargets(device, width, height);
	CreateDepthStencil(device, width, height);
	
	return true;
}

//===================== [CreateRenderTargets] =============
// Helper member to create rendertargets
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// device			- Pointer to the current display device
// width			- Horizontal resolution
// height			- Vertical resoluion
//=========================================================
bool RenderTarget::CreateRenderTargets(ID3D11Device *device, int width, int height)
{
	// Step 1: Create Render Target textures
	D3D11_TEXTURE2D_DESC textureDesc;
	
	ZeroMemory(&textureDesc, sizeof(textureDesc)); // IMPORTANT!!!!!
	
	textureDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Width				= width;
	textureDesc.Height				= height;
	textureDesc.ArraySize			= 1;
	textureDesc.MipLevels			= 1;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= 0;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// Create RT array
	for (int i = 0; i < _numRT; i++)
	{
		device->CreateTexture2D(&textureDesc, NULL, &_RenderTarget[i]);
	}

	// Step 2: Create Render Target Views
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc)); // IMPORTANT!!!!!
	
	renderTargetViewDesc.Format					= textureDesc.Format;
	renderTargetViewDesc.ViewDimension			= D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture3D.MipSlice		= 0;

	// Create Render Target View Array
	for (int i = 0; i < _numRT; i++)
	{
		device->CreateRenderTargetView(_RenderTarget[i], &renderTargetViewDesc,
			&_RTV[i]);
	}

	// Step 3: Create Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	
	srvDesc.Format						= textureDesc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= 1;

	// Create shader Resource Array
	for (int i = 0; i < _numRT; i++)
	{
		device->CreateShaderResourceView(_RenderTarget[i], &srvDesc,
			&_SRV[i]);
	}

	return true;
}

bool RenderTarget::ResizeRenderTarget(int width, int height)
{
	// Not implemented
	return false;
}

//==================================================
// Retrieve rendertargetview from this object
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
ID3D11RenderTargetView** RenderTarget::GetRenderTargetView()
{
	return _RTV;
}

//==================================================
// Retrieve render target resources for this object
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
ID3D11ShaderResourceView** RenderTarget::GetRenderTargetResource()
{
	return _SRV;
}

//==================== [getNumRT] ==================
// Return number of render targets
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
unsigned int RenderTarget::GetNumRT()
{
	return _numRT;
}

bool RenderTarget::CreateDepthStencil(ID3D11Device *device, int width, int height)
{
	ID3D11DeviceContext* context;
	
	
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	device->CreateDepthStencilState(&dsDesc, &pDSState);

	// Bind depth stencil state
	device->GetImmediateContext(&context); 
	context->OMSetDepthStencilState(pDSState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	ID3D11DepthStencilView* pDSV;
	device->CreateDepthStencilView(pDepthStencil, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&pDSV);  // [out] Depth stencil view



	// Bind the depth stencil view
	context->OMSetRenderTargets(1,          // One rendertarget view
		_RTV,      // Render target view, created earlier
		pDSV);     // Depth stencil view for the render target



	return false;
}