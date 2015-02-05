#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::Init(int screenWidth, int screenHeight, HWND handle)
{
	bool vsync = true;
	bool fullscreen = true;
	float depth = 1000.0f;
	float sNear = 0.0f;

	// Setup Child Components
	pRender = new D3D11Render;
	pRender->Init(screenWidth, screenHeight, handle, vsync, fullscreen, depth, sNear);

	pCamera = new Camera;
	pCamera->Init(90, (float)screenWidth / screenHeight, sNear, depth);

	// Insert new engine stuff here


	return true;
}

bool Renderer::Update()
{
	// Send Camera Data to Renderer
	pRender->SetCameraData(pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	pRender->Draw();

	return true;
}

bool Renderer::Release()
{
	//_render->ShutDown();

	return true;
}