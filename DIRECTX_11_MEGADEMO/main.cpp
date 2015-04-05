#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include "debug.h"
#include "Kernel.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int CmdShow)
{
	Kernel kernel(hInstance,CmdShow);

	kernel.Run();
	
	return 0;
}

