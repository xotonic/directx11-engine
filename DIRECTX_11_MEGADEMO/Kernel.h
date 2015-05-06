
#include "common.h"
#include "DXResources.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Input.h"
class Kernel
{
public:
	Kernel(HINSTANCE hInst, int nCmdShow, int w = 640, int h = 480);
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Renderer *renderer;
	DXResources* dx;
	Renderer2D *renderer2D;
	void Run();
	~Kernel();
	HWND hWnd;
private:
	HINSTANCE hInstance;
	Input *input;
	static long int _stdcall WinMessage(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);
	long int Message(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);
};
