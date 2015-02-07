#include "Effect.h"

Effect::Effect() {}
Effect::~Effect() {}

//=====[Init]===========================================
// Init the Effect object
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// device			- Pointer to current display device
// vsPath			- Filepath to vertex shader
// psPath			- Filepath to pixel shader
//======================================================
void Effect::Init(ID3D11Device* device, LPCWSTR vsPath, LPCWSTR psPath)
{
	// Store pointer to device locally
	pDevice = device;

	// Grab the immediate device context from the device
	device->GetImmediateContext(&pContext);

	// Compile shaders
	CompileEffect(vsPath, psPath);

	// Send shaders to the current device
	CreateEffect();

	// Set Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC rDes;
}

//=====[Compile Effect]=============================
// Compile shader files from disk
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// vsPath			- Filepath to the vertex shader
// psPath			- Filepath to the pixel shader
//==================================================
void Effect::CompileEffect(LPCWSTR vsPath, LPCWSTR psPath)
{
	D3DCompileFromFile(vsPath, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pBlobVS, &pBlobError);

	D3DCompileFromFile(psPath, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pBlobPS, &pBlobError);
}

//=====[CreateEffect]=================================
// Create and bind shader effects from compiled source
//
// --What if I wanted to create shaders on a different
//	 device?
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//====================================================
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

//=====[BindEffect]=================================
// Bind effects to the current device context
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
void Effect::BindEffect()
{
	// Bind Effect and resources to the pipeline
	pContext->PSSetShaderResources(0, pResourceCount, pShaderResources);
	pContext->VSSetShader(pVertexShader, NULL, 0);
	pContext->PSSetShader(pPixelShader, NULL, 0);
}

// Note: Why does the Vertex Shader need to know the input layout? //
// Answer: Because this is how the application knows what cbuffer data to send //
//=====[SendVSBlob]=============================
// Retrieve VSBlob file
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
ID3D10Blob* Effect::SendVSBlob()
{
	return pBlobVS;
}

//=====[Load Shader Resources]=============================
// Load shader resources from file.
//
// Note: A maximum of 10 shader resources per effect are
//		 supported at this time.
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// texturePath		- Filepath to the texture resource
//=========================================================
void Effect::SetShaderResources(LPCWSTR texturePath)
{
	if (pResourceCount <= MAX_RESOURCES)
	{
		CreateDDSTextureFromFile(pDevice, texturePath, NULL, &pShaderResources[pResourceCount]);
		pResourceCount++;
	}
}

//=====[SetShaderResources]=============================
// Load shader resource from memory
//
// Note: Overloaded member
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// resource			- Pointer to the shader resource
//======================================================
void Effect::SetShaderResources(ID3D11ShaderResourceView *resource)
{
	if (pResourceCount <= MAX_RESOURCES)
	{
		pShaderResources[pResourceCount] = resource;
		pResourceCount++;
	}
}