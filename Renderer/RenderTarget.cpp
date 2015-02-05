#include "RenderTarget.h"

RenderTarget::RenderTarget() {}
RenderTarget::~RenderTarget() {}

bool RenderTarget::Init(ID3D11Device *device, int width, int height, unsigned int numRT)
{
	// Set up this object
	pNumRT = numRT;
	CreateRenderTargets(device, width, height);
	
	return true;
}

bool RenderTarget::CreateRenderTargets(ID3D11Device *device, int width, int height)
{
	// Step 1: Create Render Target textures
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc)); // IMPORTANT!!!!!
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Width = 1280;
	textureDesc.Height = 720;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// Create RT array
	for (int i = 0; i < pNumRT; i++)
	{
		device->CreateTexture2D(&textureDesc, NULL, &pRenderTarget[i]);
	}

	// Step 2: Create Render Target Views
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc)); // IMPORTANT!!!!!
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture3D.MipSlice = 0;

	// Create Render Target View Array
	for (int i = 0; i < pNumRT; i++)
	{
		device->CreateRenderTargetView(pRenderTarget[i], &renderTargetViewDesc,
			&pRTV[i]);
	}

	// Step 3: Create Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create shader Resource Array
	for (int i = 0; i < pNumRT; i++)
	{
		device->CreateShaderResourceView(pRenderTarget[i], &srvDesc,
			&pSRV[i]);
	}

	return true;
}

ID3D11RenderTargetView** RenderTarget::GetRenderTargetView()
{
	return pRTV;
}

ID3D11ShaderResourceView** RenderTarget::GetRenderTargetResource()
{
	return pSRV;
}

unsigned int RenderTarget::GetNumRT()
{
	return pNumRT;
}