///=================================================================================================
// file:	InputAssembler.cpp
//
// summary:	Implements the input assembler class
///=================================================================================================

#include "InputAssembler.h"

///=======================================================================
/// <summary>Default constructor.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///=======================================================================

InputAssembler::InputAssembler()
{
	g_VertexBuffer = NULL;
	g_InputLayout = NULL;

	m_PixelShader = NULL;
	m_VertexShader = NULL;
	m_BlobVS = NULL;
	m_BlobPS = NULL;
	m_BlobError = NULL;
}

///=======================================================================
/// <summary>Destructor.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///=======================================================================

InputAssembler::~InputAssembler() {}

///=======================================================================
/// <summary>Initialises this object.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///
/// <param name="device" type="ID3D11Device*">		  [in,out] If non-null, the device.</param>
/// <param name="context" type="ID3D11DeviceContext*">[in,out] If non-null, the context.</param>
/// <param name="vertexShaderBlob" type="ID3D10Blob*">	[in,out] If non-null, the vertex shader
/// 													BLOB.</param>
///=======================================================================

void InputAssembler::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// Shader Setup

	// Deferred Shader goes here
	pDeferredMRT = new Effect();
	pDeferredMRT->Init(device, L"DefVertexShader.hlsl", L"DefPixelShader.hlsl");
	pDeferredMRT->SetShaderResources(L"BOX.DDS");
	pDeferredMRT->SetShaderResources(L"BOXNORMAL.DDS");

	// Fullscreen Quad to backbuffer
	pRTBackbuffer = new Effect();
	pRTBackbuffer->Init(device, L"VertexShader.hlsl", L"PixelShader.hlsl");
	//pRTBackbuffer->SetShaderResources(L"BOXNORMAL.DDS");

	// Setup model data
	pModel = new Model();
	pModel->Init(L"Model.txt");

	// TODO: Load defaults to init this object. There will be other methods that I can pass
	// Geometry data to load and automaticly create the vertex buffer from that data
	CreateVertexBuffer(device);
	CreateInputLayout(device);

	// Device Context setup
	UINT stride = sizeof(VertexTypeDef); // I wasted an hour on you.......
	UINT offset = 0;

	context->IASetInputLayout(g_InputLayout);
	context->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

/* Query Methods */

///=======================================================================
/// <summary>Gets input layout.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///
/// <returns>null if it fails, else the input layout.</returns>
///=======================================================================

ID3D11InputLayout* InputAssembler::GetInputLayout()
{
	return g_InputLayout;
}

///=======================================================================
/// <summary>Gets vertex buffer.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///
/// <returns>null if it fails, else the vertex buffer.</returns>
///=======================================================================

ID3D11Buffer* InputAssembler::GetVertexBuffer()
{
	return g_VertexBuffer;
}

/* HElper Methods */

///=======================================================================
/// <summary>Creates vertex buffer.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///
/// <param name="device" type="ID3D11Device*">[in,out] If non-null, the device.</param>
///=======================================================================

void InputAssembler::CreateVertexBuffer(ID3D11Device* device)
{
	using namespace DirectX;
	std::vector<VertexTypeDef> temp = pModel->GetVertexArray();

	// Stupid hackish stuff
	VertexTypeDef vertices[36];
	std::copy(temp.begin(), temp.end(), vertices);


	/* Now Create VertexBuffer!
	Step 1: Bind Vertex information to D3D11_SUBRESOURCE_DATA
	Step 2: Fill out buffer description
	Step 3: Bind Subresource data to device (copy from sys mem to video mem)
	*/


	// Bind Subresource data to device
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = vertices;

	// Fill out buffer description
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// Bind Subresource data to device
	device->CreateBuffer(&bd, &initData, &g_VertexBuffer);

#pragma region TEST SHIT
	/* INDEX BUFFER */
	unsigned short cubeIndices[] = 
	{
		0,1,2,3,4,5,6,7,8,9,10,11, 12,13,14,15,16,17,18,19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30,31,32,33,34,35,36
	};
	pModel->GetIndexArray(cubeIndices);

#pragma endregion

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(cubeIndices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// This call allocates a device resource for the index buffer and copies
	// in the data.
	device->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&g_IndexBuffer
		);
}

///=======================================================================
/// <summary>Creates input layout.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///
/// <param name="device" type="ID3D11Device*">		  [in,out] If non-null, the device.</param>
/// <param name="vertexShaderBlob" type="ID3D10Blob*">	[in,out] If non-null, the vertex shader
/// 													BLOB.</param>
///=======================================================================

void InputAssembler::CreateInputLayout(ID3D11Device* device)
{
	/* Create Input Layout
	Step 1: Fill out element description (must match vertex type
	Step 2: Bind Input Layout to device

	NOTE: Why do I have to supply the Vertex Shader to the inputlayout?
	*/

	// Fill out Input Layout element description
	// SHOULD change this depending on the vertex type used
	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = _countof(elements);

	// Bind Input Layout to device
	// NOTE: THIS WILL CAUSE PROBLEMS LATER
	ID3D10Blob *VS = pDeferredMRT->SendVSBlob();
	device->CreateInputLayout(elements, numElements, VS->GetBufferPointer(), VS->GetBufferSize(),
		&g_InputLayout);
}


///=======================================================================
/// <summary>Loads this object.</summary>
///
/// <remarks>Robert, 8/7/2014.</remarks>
///=======================================================================

void InputAssembler::Load()
{
}

//==========================================
// OLD SHADER MEMBERS
//==========================================
void InputAssembler::RenderShaders()
{	
	pRTBackbuffer->BindEffect();
}

void InputAssembler::RenderDeferred()
{
	pDeferredMRT->BindEffect();
}
void InputAssembler::SetDeferredResource(ID3D11ShaderResourceView *srv)
{
	pRTBackbuffer->SetShaderResources(srv);
}