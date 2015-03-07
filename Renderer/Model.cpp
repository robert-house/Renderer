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

void Model::GetIndexArray(unsigned short *indices)
{
	indices = new unsigned short[_IndexCount];
	
	for (int i = 0; i < _IndexCount; i++)
	{
		indices[i] = i;
	}
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
	//pModel = new ModelType[_VertexCount];

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
		temp.texcoords = XMFLOAT2(tu, tv);
		temp.normal = XMFLOAT3(nx, ny, nz);

		// Push to vector
		_vertices.push_back(temp);
		//fin >> pModel[i].x >> pModel[i].y >> pModel[i].z;
		//fin >> pModel[i].tu >> pModel[i].tv;
		//fin >> pModel[i].nx >> pModel[i].ny >> pModel[i].nz;
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
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.pos.x - vertex1.pos.x;
	vector1[1] = vertex2.pos.y - vertex1.pos.y;
	vector1[2] = vertex2.pos.z - vertex1.pos.z;

	vector2[0] = vertex3.pos.x - vertex1.pos.x;
	vector2[1] = vertex3.pos.y - vertex1.pos.y;
	vector2[2] = vertex3.pos.z - vertex1.pos.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tex.y - vertex1.tex.y;
	tvVector[0] = vertex2.tex.x - vertex1.tex.x;

	tuVector[1] = vertex3.tex.x - vertex1.tex.x;
	tvVector[1] = vertex3.tex.y - vertex1.tex.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Model::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3 &normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;


}