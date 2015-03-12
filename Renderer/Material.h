#pragma once

#include <d3d11.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Material
{
public:
	Material();
	~Material();

	void Load(string path);

	ID3D11Texture2D* getDiffuse(string);

private:
	vector<ID3D11ShaderResourceView*> _textures;
};