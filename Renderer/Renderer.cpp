#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

//================== [Init] =============================
// Initialize the Renderer object
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
// screenWidth		- Screen width in pixels
// screenHeight		- Screen height in pixels
// handle			- Pointer to target window
//==================================================
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

	_entities.push_back(new Entity("Box"));
	_entities.push_back(new Entity("Sphere"));
	_entities.push_back(new Entity("Pyrmid"));

	// Load data for box
	_entities[0]->setPosition(Vector3(0, 0, 0));
	_entities[0]->setRotation(Vector3(0, 0, 0));
	_entities[0]->setSize(1.0f);
	_entities[0]->Load(L"box.txt");


	// Insert new engine stuff here


	return true;
}

//==================================================
// Main update loop
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
bool Renderer::Update()
{
	// Update Camera location
	//Vector3 temp(0.01f, 0.0f, 0.0f);
	//pCamera->OffsetCamera(temp);
	
	// Send Camera Data to Renderer
	pRender->SetCameraData(pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	pRender->Draw();

	return true;
}

//==================================================
// Shutdown and release memory
//--------------	--------------------------------
//|   Params   |	|		  Description          |
//--------------	--------------------------------
//==================================================
bool Renderer::Release()
{
	//_render->ShutDown();

	return true;
}