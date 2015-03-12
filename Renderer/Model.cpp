#include "Model.h"

Model::Model() {}
Model::~Model() {}

bool Model::Init(LPCWSTR fileName)
{
	LoadFromFile(fileName);

	return true;
}

bool Model::Release()
{
	return true;
}

std::vector <VertexTypeDef> Model::GetVertexArray()
{
	/*using namespace DirectX;
	std::vector<VertexTypeDef> arr;
	VertexTypeDef temp;

	for (int i = 0; i < _VertexCount; i++)
	{
		temp.position	= XMFLOAT3(pModel[i].z, pModel[i].y, pModel[i].x);
		temp.texcoords	= XMFLOAT2(pModel[i].tu, pModel[i].tv);
		temp.normal		= XMFLOAT3(pModel[i].nz, pModel[i].ny, pModel[i].nx);

		arr.push_back(temp);
	}*/

	return _vertices;
}

vector<unsigned short> Model::GetIndexArray()
{
	return _indices;
}

unsigned int Model::getNumVerts()
{
	return _VertexCount;
}

unsigned int Model::getNumIndex()
{
	return _IndexCount;
}

bool Model::LoadFromFile(LPCWSTR fileName)
{
	// NOTE: Have to swap (X Z) and (tu tv) as the mesh is stored
	// lefthanded?
	
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

	//Read up to where the data is
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the data
	float x, y, z, tu, tv, nx, ny, nz;

	for (i = 0; i < _VertexCount; i++)
	{
		// Read in the entire vertex
		fin >> z >> y >> x >> tu >> tv >> nz >> ny >> nx;
		
		// Create Vertex Struct
		VertexTypeDef temp;
		temp.position = XMFLOAT3(x, y, z);
		temp.texcoords = XMFLOAT2(tv, tu);
		temp.normal = XMFLOAT3(nx, ny, nz);

		// Push to vector
		_vertices.push_back(temp);
	}

	// Indices Come Next
	unsigned int temp;

	while (!fin.eof())
	{
		fin >> temp;
		_indices.push_back(temp);
		_IndexCount++;
	}

	// Close file
	fin.close();

	CalculateVectors();

	return true;
}

void Model::CalculateVectors()
{
	int facecount = _VertexCount / 3;
	int index = 0;

	TempVert vert1, vert2, vert3;
	Vector3 tangent, binormal, normal;

	for (int i = 0; i < facecount; i++)
	{
		// Get the three vertices for this face from the model.
		vert1.pos = _vertices[index].position;
		vert1.tex = _vertices[index].texcoords;
		vert1.norm = _vertices[index].normal;
		index++;

		vert2.pos = _vertices[index].position;
		vert2.tex = _vertices[index].texcoords;
		vert2.norm = _vertices[index].normal;
		index++;

		vert3.pos = _vertices[index].position;
		vert3.tex = _vertices[index].texcoords;
		vert3.norm = _vertices[index].normal;
		index++;

		CalculateTangentBiNormal(vert1, vert2, vert3, tangent, binormal);
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		_vertices[index - 1].normal = normal;
		_vertices[index - 1].tangent = tangent;
		_vertices[index - 1].binormal = binormal;

		_vertices[index - 2].normal = normal;
		_vertices[index - 2].tangent = tangent;
		_vertices[index - 2].binormal = binormal;

		_vertices[index - 3].normal = normal;
		_vertices[index - 3].tangent = tangent;
		_vertices[index - 3].binormal = binormal;


	}
}

void Model::CalculateTangentBiNormal(TempVert vertex1, TempVert vertex2, TempVert vertex3,
	Vector3& tangent, Vector3& binormal)
{
	Vector3 vec1, vec2;
	Vector2 tu, tv;
	float coef;


	// Calculate the two vectors for this face.
	vec1.x = vertex2.pos.x - vertex1.pos.x;
	vec1.y = vertex2.pos.y - vertex1.pos.y;
	vec1.z = vertex2.pos.z - vertex1.pos.z;

	vec2.x = vertex3.pos.x - vertex1.pos.x;
	vec2.y = vertex3.pos.y - vertex1.pos.y;
	vec2.z = vertex3.pos.z - vertex1.pos.z;

	// Calculate the tu and tv texture space vectors.
	tu.x = vertex2.tex.x - vertex1.tex.x;
	tv.x = vertex2.tex.y - vertex1.tex.y;

	tu.y = vertex3.tex.x - vertex1.tex.x;
	tv.y = vertex3.tex.y - vertex1.tex.y;

	// Calculate the coefficent of the tangent/binormal equation.
	coef = 1.0f / (tu.x * tv.y - tu.y * tv.x);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tv.y * vec1.x - tv.x * vec2.x) * coef;
	tangent.y = (tv.y * vec1.y - tv.x * vec2.y) * coef;
	tangent.z = (tv.y * vec1.z - tv.x * vec2.z) * coef;

	binormal.x = (tu.x * vec2.x - tu.y * vec1.x) * coef;
	binormal.y = (tu.x * vec2.y - tu.y * vec1.y) * coef;
	binormal.z = (tu.x * vec2.z - tu.y * vec1.z) * coef;

	// Normalize the Binormal and Tangent Vectors
	tangent.Normalize();
	binormal.Normalize();
}

void Model::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3 &normal)
{
	// Calculate the cross product of the tangent and binormal
	normal = tangent.Cross(binormal);

	// Normalize the normal.
	normal.Normalize();
}

VS_CBUFFER_PER_OBJECT Model::getCBuffer()
{
	return _cbuffer;
}