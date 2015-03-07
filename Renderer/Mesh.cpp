#include "Mesh.h"

Mesh::Mesh() {}
Mesh::~Mesh() {}

bool Mesh::Init(LPCWSTR fileName)
{
	LoadFromFile(fileName);

	return true;
}

bool Mesh::Release()
{
	return true;
}

std::vector <VertexTypeDef> Mesh::GetVertexArray()
{
	using namespace DirectX;
	std::vector<VertexTypeDef> arr;
	VertexTypeDef temp;

	for (int i = 0; i < _VertexCount; i++)
	{
		temp.position	= XMFLOAT3(pModel[i].z, pModel[i].y, pModel[i].x);
		temp.texcoords	= XMFLOAT2(pModel[i].tu, pModel[i].tv);
		temp.normal		= XMFLOAT3(pModel[i].nz, pModel[i].ny, pModel[i].nx);

		arr.push_back(temp);
	}

	return arr;
}

void Mesh::GetIndexArray(unsigned short *indices)
{
	indices = new unsigned short[_IndexCount];
	
	for (int i = 0; i < _IndexCount; i++)
	{
		indices[i] = i;
	}
}

unsigned int Mesh::getNumVerts()
{
	return _VertexCount;
}

unsigned int Mesh::getNumIndex()
{
	return _IndexCount;
}

bool Mesh::LoadFromFile(LPCWSTR fileName)
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
	fin >> _VertexCount;

	// Set index count to be the same as the vertex count
	_IndexCount = _VertexCount;

	// Create model array
	pModel = new ModelType[_VertexCount];

	//Read up to where the data is
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the data
	for (i = 0; i < _VertexCount; i++)
	{
		fin >> pModel[i].x >> pModel[i].y >> pModel[i].z;
		fin >> pModel[i].tu >> pModel[i].tv;
		fin >> pModel[i].nx >> pModel[i].ny >> pModel[i].nz;
	}

	// Close file
	fin.close();

	return true;
}