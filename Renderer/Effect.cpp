#include "Effect.h"

Effect::Effect() {}
Effect::~Effect() {}

void Effect::Init(ID3D11Device* device, LPCWSTR vsPath, LPCWSTR psPath)
{
	pDevice = device;
	device->GetImmediateContext(&pContext);
	CompileEffect(vsPath, psPath);
	CreateEffect();

	// Set Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC rDes;
}

void Effect::CompileEffect(LPCWSTR vsPath, LPCWSTR psPath)
{
	D3DCompileFromFile(vsPath, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pBlobVS, &pBlobError);

	D3DCompileFromFile(psPath, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pBlobPS, &pBlobError);
}

void Effect::CreateEffect()
{
	// Check if compiled shaders have been stored in the blobs, if so
	// Bind the Shaders to the shader objects
	if (pBlobVS != NULL)
	{
		pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(),
			NULL, &pVertexShader);
	}

	if (pBlobPS != NULL)
	{
		pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(),
			NULL, &pPixelShader);
	}
}

void Effect::BindEffect()
{
	// Bind Effect to the pipeline
	pContext->PSSetShaderResources(0, pResourceCount, pShaderResources);

	pContext->VSSetShader(pVertexShader, NULL, 0);
	pContext->PSSetShader(pPixelShader, NULL, 0);
}

// Note: Why does the Vertex Shader need to know the input layout? //
// Answer: Because this is how the application knows what cbuffer data to send //
ID3D10Blob* Effect::SendVSBlob()
{
	return pBlobVS;
}

void Effect::SetShaderResources(LPCWSTR texturePath)
{
	using namespace DirectX;

	if (pResourceCount <= 10)
	{
		CreateDDSTextureFromFile(pDevice, texturePath, NULL, &pShaderResources[pResourceCount]);
		pResourceCount++;
	}
}

void Effect::SetShaderResources(ID3D11ShaderResourceView *resource)
{
	if (pResourceCount <= 10)
	{
		pShaderResources[pResourceCount] = resource;
		pResourceCount++;
	}
}