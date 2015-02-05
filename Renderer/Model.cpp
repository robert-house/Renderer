#include "Model.h"

Model::Model() {}
Model::~Model() {}

bool Model::Init(LPCWSTR fileName)
{
	Load(fileName);

	return true;
}

bool Model::Release()
{
	return true;
}

std::vector <VertexTypeDef> Model::GetVertexArray()
{
	using namespace DirectX;
	std::vector<VertexTypeDef> arr;
	VertexTypeDef temp;

	for (int i = 0; i < pVertexCount; i++)
	{
		temp.position = XMFLOAT3(pModel[i].z, pModel[i].y, pModel[i].x);
		temp.texcoords = XMFLOAT2(pModel[i].tu, pModel[i].tv);
		temp.normal = XMFLOAT3(pModel[i].nz, pModel[i].ny, pModel[i].nx);

		arr.push_back(temp);
	}

	return arr;
}

void Model::GetIndexArray(unsigned short *indices)
{
	indices = new unsigned short[pIndexCount];
	
	for (int i = 0; i < pIndexCount; i++)
	{
		indices[i] = i;
	}
}

bool Model::Load(LPCWSTR fileName)
{
	std::ifstream fin;
	char input;
	int i = 0;

	fin.open(fileName);

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//Get Vertex Count
	fin >> pVertexCount;

	// Set index count to be the same as the vertex count
	pIndexCount = pVertexCount;

	// Create model array
	pModel = new ModelType[pVertexCount];

	//Read up to where the data is
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the data
	for (i = 0; i<pVertexCount; i++)
	{
		fin >> pModel[i].x >> pModel[i].y >> pModel[i].z;
		fin >> pModel[i].tu >> pModel[i].tv;
		fin >> pModel[i].nx >> pModel[i].ny >> pModel[i].nz;
	}

	// Close file
	fin.close();

	return true;
}