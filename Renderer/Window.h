#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Renderer.h"

//Fields
int screenWidth;
int screenHeight;
Renderer* pRenderer = 0;
HWND mainWindow = 0;
FILE *stream;

#pragma region Function Prototypes
//Main Entry Point
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd);

// Windows initialization
bool InitWindows(HINSTANCE hInstance, int show);

// Contains Main Loop
int Run();

// Handles windows events
LRESULT CALLBACK
WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
#pragma endregion