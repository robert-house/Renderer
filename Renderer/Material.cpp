#include "Material.h"

Material::Material() {}
Material::~Material() {}

void Material::Load(string path)
{
	//CreateDDSTextureFromFile(pDevice, texturePath, NULL, &pShaderResources[_numMaterials]);
	_numMaterials++;
}