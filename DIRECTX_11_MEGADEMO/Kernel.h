#include "stdafx.h"
#include "DXResources.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Input.h"
#include "Timer.h"

class Kernel
{
public:
	Kernel(HINSTANCE hInst, int nCmdShow, int w = 640, int h = 480);
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<DXResources> dx;
	std::shared_ptr<Renderer2D> renderer2D;
	std::shared_ptr<Input> input;
	Timer timer;
	void Run();
	HWND hWnd;
private:
	int x, y;
	HINSTANCE hInstance;
	static long int _stdcall WinMessage(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);
	long int Message(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);
};
