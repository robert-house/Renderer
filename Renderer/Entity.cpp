#include "Entity.h"

Entity::Entity(string name) {}
Entity::~Entity() {}

bool Entity::Load(LPCWSTR filePath)
{
	_mesh = new Mesh();
	_mesh->Init(filePath);

	return true;
}

bool Entity::Update()
{
	BuildWorldMatrix();

	return true;
}

XMFLOAT4X4 Entity::getWorldMatrix()
{
	return _worldMatrix;
}

Mesh* Entity::getMesh()
{
	return _mesh;
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

bool Entity::BuildWorldMatrix()
{
	XMStoreFloat4x4(&_worldMatrix,
		XMMatrixTranspose(XMMatrixScaling(_size, _size, _size)
		* XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z)
		* XMMatrixTranslation(_position.x, _position.y, _position.z)));

	return true;
}