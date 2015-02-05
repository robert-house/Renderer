//--------------------------------------------------------------------------------------
// File: Camera.h
//
// Camera compnent
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------


#include "ICOMDrawable.h"
#include <DirectXMath.h>

class Camera : public ICOMDrawable
{
public:
	Camera();
	virtual ~Camera();

	virtual void Init();
	void Init(float fov, float aspectRatio, float screenNear, float screenDepth);
	virtual void Update();
	virtual void ShutDown();
	virtual void Draw();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();
	void CreateProjectionMatrix();
	void CreateViewMatrix();

private:
	struct position
	{
		float x, y, z;
	};

	DirectX::XMFLOAT4X4 g_ViewMatrix;
	DirectX::XMFLOAT4X4 g_ProjMatrix;
	float g_phi;
	float g_theta = 1.5f * DirectX::XM_PI;
	float g_radius;
	float g_aspectRatio;
	float g_fov;
	float g_screenNear;
	float g_screenDepth;

	/* Test Methods */
public:
	void MoveCameraPosition();
	float count = 0;

};