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
	_ia = pRender->RegisterIA();

	pCamera = new Camera;
	pCamera->Init(90, (float)screenWidth / screenHeight, sNear, depth);
	pCamera->MoveCamera(Vector3(3, 1, 4));

	_entities.push_back(new EntityDrawable("Box"));
	_entities.push_back(new EntityDrawable("Sphere"));
	_entities.push_back(new EntityDrawable("Pyrmid"));

	// Load data for meshes. This is hard coded here
	// as my resource manager is NOT yet ready

	// Box
	_entities[0]->setPosition(Vector3(0, 0, 0));
	_entities[0]->setRotation(Vector3(0, 0, 0));
	_entities[0]->setSize(1.0f);
	_entities[0]->Load(L"box.txt");

	// Sphere
	_entities[1]->setPosition(Vector3(-2, 0, 0));
	_entities[1]->setRotation(Vector3(0, 0, 0));
	_entities[1]->setSize(1.0f);
	_entities[1]->Load(L"box.txt");

	// Pyramid
	_entities[2]->setPosition(Vector3(2, 0, 0));
	_entities[2]->setRotation(Vector3(0, 0, 0));
	_entities[2]->setSize(1.0f);
	_entities[2]->Load(L"box.txt");


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
	_entities[0]->setRotation(Vector3((count * XM_PI) / 180.0f, 0, 0));
	_entities[0]->Update();

	_entities[1]->setRotation(Vector3(0, (count * XM_PI) / 180.0f, 0));
	_entities[1]->Update();

	_entities[2]->setRotation(Vector3(0, 0, (count * XM_PI) / 180.0f));
	_entities[2]->Update();

	pRender->ClearRTs();

	pRender->SetCameraData(pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	
	for (int i = 2; i >= 0; i--)
	{
		// Add Entities to draw queue
		_ia->_model = _entities[i]->getModel();

		// Send Camera Data to Renderer
		pRender->setWorldMatrix(_entities[i]->getWorldMatrix());

		// Create new vertex buffer with the next data
		_ia->BuildVertexBuffer();

		pRender->Update();

		// Draw scene
		pRender->Draw();
	}

	count = 1.0f;

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