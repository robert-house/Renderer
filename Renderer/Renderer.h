#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include "D3D11Render.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "Entity.h"

using namespace SimpleMath;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Init(int screenWidth, int screenHeight, HWND windowHandle);
	bool Update();
	bool Release();

private:
	D3D11Render* pRender;
	Camera* pCamera;
	vector<Entity*> _entities;
};

#endif