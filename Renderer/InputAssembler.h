#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "VertexTypes.h"
#include "DDSTextureLoader.h"
#include "Effect.h"
#include "Model.h"

#pragma once
#pragma comment (lib, "d3dcompiler.lib")

class InputAssembler
{
	/* Globals */
	ID3D11Buffer*					g_VertexBuffer;
	ID3D11Buffer*					g_IndexBuffer;
	ID3D11InputLayout*				g_InputLayout;
	D3D11_PRIMITIVE_TOPOLOGY		g_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ID3D11VertexShader*				m_VertexShader;
	ID3D11PixelShader*				m_PixelShader;
	ID3D10Blob*						m_BlobVS;
	ID3D10Blob*						m_BlobPS;
	ID3D10Blob*						m_BlobError;

	/* Methods */
public:
	InputAssembler();
	~InputAssembler();

	void							Init(ID3D11Device* device, ID3D11DeviceContext* context);
	void							CreateVertexBuffer(ID3D11Device* device);
	void							CreateInputLayout(ID3D11Device* device);
	void							Load();

	void							CompileShaders(LPCWSTR filepath);
	void							BindShaders(ID3D11Device* device);
	void							RenderShaders();
	void							CreateShaderResourceView(ID3D11Device* device);
	void RenderDeferred();
	void SetDeferredResource(ID3D11ShaderResourceView *srv);

	/* Query Methods*/
public:
	ID3D11VertexShader*				GetVertexShader();
	ID3D11PixelShader*				GetPixelShader();
	ID3D11InputLayout*				GetInputLayout();
	ID3D11Buffer*					GetVertexBuffer();

	// Test shit
private:
	Effect *pRTBackbuffer;
	Effect *pDeferredMRT;
	Model *pModel;
	

};