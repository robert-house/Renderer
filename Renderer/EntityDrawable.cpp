#include "EntityDrawable.h"

EntityDrawable::EntityDrawable(string name) : Entity(name)
{

}

EntityDrawable::~EntityDrawable()
{

}

bool EntityDrawable::Load(LPCWSTR path)
{
	// Load model here
	_model = new Model();
	_model->Init(path);

	return true;
}

bool EntityDrawable::Update()
{
	BuildWorldMatrix();

	return true;
}

XMFLOAT4X4 EntityDrawable::getWorldMatrix()
{
	return _worldMatrix;
}

Model* EntityDrawable::getModel()
{
	return _model;
}

bool EntityDrawable::BuildWorldMatrix()
{
	XMStoreFloat4x4(&_worldMatrix,
		XMMatrixTranspose(XMMatrixScaling(_size, _size, _size)
		* XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z)
		* XMMatrixTranslation(_position.x, _position.y, _position.z)));

	return true;
}