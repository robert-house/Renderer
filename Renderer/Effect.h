#ifndef EFFECT_H
#define EFFECT_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include "DDSTextureLoader.h"

class Effect
{
	static const unsigned int MAX_RESOURCES = 10;

public:
	Effect();
	~Effect();
	void Init(ID3D11Device* device, LPCWSTR vsPath, LPCWSTR psPath);
	void CompileEffect(LPCWSTR vsPath, LPCWSTR psPath);
	void SetCBuffer(ID3D11Buffer *buffer);
	void SetShaderResources(LPCWSTR texturePath);
	void SetShaderResources(ID3D11ShaderResourceView *resource);
	void CreateEffect();
	void BindEffect();
	ID3D10Blob* SendVSBlob();

private:
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pContext;
	ID3D11InputLayout *pInputLayout;
	ID3D10Blob* pBlobPS = NULL;
	ID3D10Blob* pBlobVS = NULL;
	ID3D10Blob* pBlobError;
	ID3D11PixelShader *pPixelShader;
	ID3D11VertexShader *pVertexShader;
	ID3D11ShaderResourceView *pShaderResources[MAX_RESOURCES];
	unsigned int pResourceCount = 0;
};

#endif