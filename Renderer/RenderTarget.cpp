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
	//CreateDepthStencil(device, width, height);
	
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