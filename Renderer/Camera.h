//--------------------------------------------------------------------------------------
// File: Camera.h
//
// Camera compnent
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class Camera
{
private:
	Vector3			_position;
	Vector3			_target;
	Vector3			_up;
	Matrix			_ViewMatrix;
	Matrix			_ProjMatrix;
	float			_phi;
	float			_theta = 1.5f * XM_PI;
	float			_radius;
	float			_aspectRatio;
	float			_fov;
	float			_nearPlane;
	float			_farPlane;
	float			_cameraSensitivity = 1.0f;
	float			_apetureSize = 1.0f;

public:
	Camera();
	virtual ~Camera();

	void Init();
	void Init(float fov, float aspectRatio, float nearPlaneValue, float farPlaneValue);
	void Release();

	// Mutators
	bool MoveCamera(Vector3 position);
	bool OffsetCamera(Vector3 offset);
	bool RotateCamera(XMFLOAT3 *axis, float angle);
	bool SetFOV(float fov);
	bool SetNearPlane(float nearPlaneValue);
	bool SetFarPlane(float farPlaneValue);
	bool SetCameraSensitivity(float sensitivity);
	bool SetApetureSize(float aptSize);

	// Accessors
	XMFLOAT3 GetCameraLocation();
	XMFLOAT3 GetCameraRotation();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjMatrix();
	float GetFOV();
	float GetNearPlane();
	float GetFarPlane();
	float GetCameraVelocityMultiplier();
	float GetApetureSize();
	
private:
	// Helpers
	void CreateProjectionMatrix();
	void CreateViewMatrix();
};