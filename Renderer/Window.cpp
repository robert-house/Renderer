/************************************************************************
Main.cpp - Main entry point for Engine
- Contains Windows template to set up a rendering space

Author: Robert House 2014
*************************************************************************/

#include "Window.h"

// Main Entry Point
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
PSTR pCMDLine, int nShowCmd)
{
	// Hooks for windows Console
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "freopen.out", "w", stdout);
	fprintf(stdout, "Hello World!");

	//Set Screen Size
	screenWidth = 1280;
	screenHeight = 720;

	//initialize windows. Return 0 if not successful
	if (!InitWindows(hInstance, nShowCmd))
		return 0;

	//initialize Engine
	pRenderer = new Renderer();
	pRenderer->Init(screenWidth, screenHeight, mainWindow);

	// Loop until WM_QUIT is recieved
	Run();

	// Shutdown engine when finished
	pRenderer->Release();

	return 0;
}

// Initialization function
bool InitWindows(HINSTANCE hInstance, int show)
{
#pragma region WNDCLASS Struct Characteristics
	//Fill out characteristics of WNDCLASS struct
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "fgs";
#pragma endregion

#pragma region Window Registration
	//Register WNDCLASS
	if (!RegisterClass(&wc))
	{
		MessageBox(0, "FAILED to register class", 0, 0);
		return false;
	}
#pragma endregion

#pragma region Create Window
	mainWindow = CreateWindow(
		"fgs",		// Registered WNDCLASS to use
		"Frame Time: ",		// Window Title
		WS_OVERLAPPEDWINDOW,	// style flags
		1,						// Start x-coord
		1,						// Start y-coord
		screenWidth,			// Window width
		screenHeight,			// Window height
		0,						// Parent window
		0,						// Menu handle
		hInstance,				// Instance handle
		0);						// Extra params
#pragma endregion
	if (mainWindow == 0)
	{
		MessageBox(0, "FAILED to create window", 0, 0);
		return false;
	}

	ShowWindow(mainWindow, show);
	UpdateWindow(mainWindow);

	return true;
}

//Main Program Loop
int Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		// Process messages
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// TODO: INSERT GAME CLASS CODE HERE
			pRenderer->Update();
		}
	}

	return (int)msg.wParam;
}

// Handle Messages
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	// Handle Messages
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(mainWindow);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}