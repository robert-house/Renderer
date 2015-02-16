//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// Camera compnent
//
// Author: Robert House 2015
//--------------------------------------------------------------------------------------

#include "Camera.h"

Camera::Camera(){}
Camera::~Camera() {}

void Camera::Init() {}

void Camera::Init(float fov, float aspectRatio, float nearPlaneValue, 
	float farPlaneValue)
{
	// Set basic parameters
	_fov = fov * XM_PI / 180.0f;
	_aspectRatio = aspectRatio;
	_nearPlane = nearPlaneValue;
	_farPlane = farPlaneValue;

	// Set the position of the camera
	_position = Vector3(1.0f, 1.0f, 1.0f);

	// Set value of the lookAt vector to the origin
	_target = Vector3(0.0f, 0.0f, 0.0f);
	_up = Vector3(0.0f, 1.0f, 0.0f);

	// Construct View and Projection matrices
	CreateProjectionMatrix();
	CreateViewMatrix();

	SetFOV(120);
	//MoveCamera(Vector3(-1.0, 0.0, 1.0));
}

void Camera::Release() {}

// MUTATORS
bool Camera::MoveCamera(Vector3 position)
{
	// Update Camera position matrix
	_position = position;

	// Set direction of the lookAt vector
	

	// Rebuild View matrix
	CreateViewMatrix();
	return true;
}

bool Camera::OffsetCamera(Vector3 offset)
{
	_position += offset;
	//_target += _position;

	CreateViewMatrix();
	return false;
}

bool Camera::RotateCamera(XMFLOAT3* axis, float angle)
{
	// Convert float3 to vector
	//XMVECTOR rotation = XMLoadFloat3(axis);
	//XMMatrixRotationAxis(rotation, angle);
	return false;
}

bool Camera::SetFOV(float fov)
{
	_fov = fov * XM_PI / 180.0f;;

	// Rebuild Projection matrix
	CreateProjectionMatrix();

	return true;
}

bool Camera::SetNearPlane(float nearPlaneValue)
{
	_nearPlane = nearPlaneValue;

	// Rebuild projection matrix
	CreateProjectionMatrix();

	return true;
}

bool Camera::SetFarPlane(float farPlaneValue)
{
	_farPlane = farPlaneValue;

	// Rebuild Projection Matrix
	CreateProjectionMatrix();

	return true;
}

bool Camera::SetCameraSensitivity(float sensitivity)
{
	_cameraSensitivity = sensitivity;

	return true;
}

bool Camera::SetApetureSize(float aptSize)
{
	_apetureSize = aptSize;

	return true;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return _ViewMatrix;
}

XMFLOAT4X4 Camera::GetProjMatrix()
{
	return _ProjMatrix;
}

void Camera::CreateViewMatrix()
{
	XMStoreFloat4x4(
		&_ViewMatrix,
		XMMatrixTranspose(XMMatrixLookAtRH(_position, _target, _up)));
}

void Camera::CreateProjectionMatrix()
{
	XMStoreFloat4x4(&_ProjMatrix,
		XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(_fov, _aspectRatio, _nearPlane, _farPlane)));
}

