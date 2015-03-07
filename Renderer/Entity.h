#pragma once

#include <string>
//#include "ResourceManager.h"
#include "Mesh.h"
#include "SimpleMath.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class Entity
{
public:
	Entity(string name);
	~Entity();

	bool Load(LPCWSTR s);
	bool Update();

	XMFLOAT4X4 getWorldMatrix();
	Mesh* getMesh();

	bool setPosition(Vector3 vec);
	bool setRotation(Vector3 vec);
	bool setSize(float size);


private:
	//ResourceManager*	_rm;
	Vector3				_position;
	Vector3				_rotation;
	float				_size;
	string				_name;
	Mesh*				_mesh;
	XMFLOAT4X4			_worldMatrix;

	bool BuildWorldMatrix();
};