//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// Camera compnent
//
// Author: Robert House 2014
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
	_position = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);

	// Construct View and Projection matrices
	CreateProjectionMatrix();
	CreateViewMatrix();
}

void Camera::Release() {}

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
	// Create the constant buffer data in system memory.
	XMVECTOR eye	= XMVectorSet(0.0f,  0.7f, 1.5f, 0.0f);
	XMVECTOR up		= XMVectorSet(0.0f,  0.0f, 1.0f, 0.0f);

	XMStoreFloat4x4(
		&_ViewMatrix,
		XMMatrixTranspose(XMMatrixLookAtRH(eye, _position, up)));
}
void Camera::CreateProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(&g_ProjMatrix,
		XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(g_fov, g_aspectRatio, g_screenNear, g_screenDepth)));
}

