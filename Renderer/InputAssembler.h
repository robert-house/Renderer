#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <queue>
#include "CommonTypes.h"
#include "DDSTextureLoader.h"
#include "Effect.h"
#include "Model.h"
#include "EntityDrawable.h"

#pragma once
#pragma comment (lib, "d3dcompiler.lib")

/* TAGGED FOR STYLE CLEAN UP */
// Do not pass device context. Get context from device
//

class InputAssembler
{
	ID3D11Buffer*					g_VertexBuffer;
	ID3D11Buffer*					g_IndexBuffer;
	ID3D11InputLayout*				g_InputLayout;
	D3D11_PRIMITIVE_TOPOLOGY		g_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ID3D11VertexShader*				m_VertexShader;
	ID3D11PixelShader*				m_PixelShader;
	ID3D10Blob*						m_BlobVS;
	ID3D10Blob*						m_BlobPS;
	ID3D10Blob*						m_BlobError;

	ID3D11Device*					_device;
	ID3D11DeviceContext*			_context;
	
	/* Methods */
public:
	InputAssembler();
	~InputAssembler();

	void							Init(ID3D11Device* device, ID3D11DeviceContext* context);
	void							BuildVertexBuffer();
	void							CreateInputLayout(ID3D11Device* device);
	void							Load();

	void							CompileShaders(LPCWSTR filepath);
	void							BindShaders(ID3D11Device* device);
	void							BindShaders();
	void							CreateShaderResourceView(ID3D11Device* device);
	void							BindDeferredShaders();
	void							SetDeferredResource(ID3D11ShaderResourceView *srv);

	/* Query Methods*/
public:
	ID3D11VertexShader*				GetVertexShader();
	ID3D11PixelShader*				GetPixelShader();
	ID3D11InputLayout*				GetInputLayout();
	ID3D11Buffer*					GetVertexBuffer();
	bool							AddToDrawQueue(EntityDrawable *model);
	// Delete
	Model*							_model;

	// Test
private:
	Effect							*pRTBackbuffer;
	Effect							*pDeferredMRT;
	bool _vertexBufferCreated;
	void BatchGeometry();
	queue<EntityDrawable*> _drawQueue;

	// Geometry Batching Test
private:
	vector<VertexTypeDef> _vertices;
	vector<unsigned short> _indices;
	vector<unsigned int> _offsetBuffer;
	vector<XMFLOAT4X4> _cBufferPerObject;
};