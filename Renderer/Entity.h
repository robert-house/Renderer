#pragma once

#include <string>
//#include "ResourceManager.h"
#include "SimpleMath.h"
#include "Model.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class Entity
{
public:
	Entity(string name);
	~Entity();

	virtual bool Load(LPCWSTR path);
	virtual bool Update();

	bool setPosition(Vector3 vec);
	bool setRotation(Vector3 vec);
	bool setSize(float size);

	string getName();

public:
	Vector3				_position;
	Vector3				_rotation;
	float				_size;
	string				_name;
	
};