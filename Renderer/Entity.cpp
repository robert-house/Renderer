#include "Entity.h"

Entity::Entity(string name) {}
Entity::~Entity() {}

bool Entity::Update()
{
	return true;
}

bool Entity::Load(LPCWSTR path)
{
	return true;
}

bool Entity::setPosition(Vector3 vec)
{
	_position = vec;
	return true;
}

bool Entity::setRotation(Vector3 vec)
{
	_rotation = vec;
	return true;
}

bool Entity::setSize(float size)
{
	_size = size;
	return true;
}