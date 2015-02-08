//--------------------------------------------------------------------------------------
// File: Camera.h
//
// Camera compnent
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Init();
	void Init(float fov, float aspectRatio, float nearPlaneValue, float farPlaneValue);
	void Release();

	// Mutators
	bool MoveCamera(XMFLOAT3 position);
	bool OffsetCamera(XMFLOAT3 offset);
	bool RotateCamera();
	bool SetFOV(float fov);
	bool SetNearPlane(float nearPlaneValue);
	bool SetFarPlane(float farPlaneValue);
	bool SetCameraVelocity(float speed);
	bool SetApetureSize(float aptSize);

	// Accessors
	XMFLOAT3 GetCameraLocation();
	XMFLOAT3 GetCameraRotation();
	float GetFOV();
	float GetNearPlane();
	float GetFarPlane();
	float GetCameraVelocityMultiplier();
	float GetApetureSize();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjMatrix();
	void CreateProjectionMatrix();
	void CreateViewMatrix();

private:
	XMVECTOR _position;
	XMFLOAT4X4 _ViewMatrix;
	XMFLOAT4X4 _ProjMatrix;
	float _phi;
	float _theta = 1.5f * XM_PI;
	float _radius;
	float _aspectRatio;
	float _fov;
	float _nearPlane;
	float _farPlane;
	float _cameraVelocityMultiplier = 1.0f;
	float _apetureSize = 1.0f;

	/* Test Methods */
public:
	void MoveCameraPosition();
	float count = 0;

};