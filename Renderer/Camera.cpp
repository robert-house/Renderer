//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// Camera compnent
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

#include "Camera.h"

Camera::Camera() : ICOMDrawable(){}
Camera::~Camera() {}

void Camera::Init() {}
void Camera::Init(float fov, float aspectRatio, float screenNear, float screenDepth)
{
	g_fov = fov * XM_PI / 180.0f;
	g_aspectRatio = aspectRatio;
	g_screenNear = screenNear;
	g_screenDepth = screenDepth;
	CreateProjectionMatrix();
	CreateViewMatrix();
}
void Camera::Update()
{
	// Testing Camera Movement here
	count += .01;
	// Create the constant buffer data in system memory.
	XMVECTOR eye	= XMVectorSet(0.0f,  0.7f, 1.5f, 0.0f);
	XMVECTOR at		= XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up		= XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(
		&g_ViewMatrix,
		XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up))
		);
}
void Camera::ShutDown() {}
void Camera::Draw()
{
	// This class should know how to draw itself

}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return g_ViewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjMatrix()
{
	return g_ProjMatrix;
}

void Camera::CreateViewMatrix()
{
	// Create the constant buffer data in system memory.
	XMVECTOR eye = XMVectorSet(0.0f, 0.7f, 1.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(
		&g_ViewMatrix,
		XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up))
		);
}
void Camera::CreateProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(&g_ProjMatrix,
		XMMatrixTranspose(
		XMMatrixPerspectiveFovRH(g_fov, g_aspectRatio, g_screenNear, g_screenDepth)));
}

