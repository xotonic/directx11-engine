#include "Kernel.h"

void Kernel::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer->Render();
		}
	}

}


Kernel::Kernel(HINSTANCE hInst, int nCmdShow, int w, int h)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &WinMessage;     // points to window procedure 
	wcex.cbClsExtra = 0;                // no extra class memory 
	wcex.cbWndExtra = 0;                // no extra window memory 
	wcex.hInstance = hInst;         // handle to instance 
	wcex.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);              // predefined app. icon 
	wcex.hCursor = LoadCursor(NULL,
		IDC_ARROW);                    // predefined arrow               
	wcex.lpszMenuName = "MainMenu";    // name of menu resource 
	wcex.lpszClassName = "MainWClass";  // name of window class 
	wcex.hIconSm = 0; 
	if (!RegisterClassEx(&wcex)) DEBUG("UNABLE TO REGISTER WINAPI CLASS\n");


	hInstance = hInst;
	hWnd = CreateWindow(
		"MainWClass",        // name of window class 
		"1st kernel",            // title-bar string 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // top-level window 
		CW_USEDEFAULT,       // default horizontal position 
		CW_USEDEFAULT,       // default vertical position 
		w,       // default width 
		h,       // default height 
		(HWND)NULL,         // no owner window 
		(HMENU)NULL,        // use class menu 
		hInstance,           // handle to application instance 
		this);      // no window-creation data 

	SetWindowLong(hWnd, GWL_USERDATA, (long int)this);

	if (!hWnd) DEBUG("UNABLE TO CREATE WINDOW\n");

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	WindowDescriptor wd = { hWnd, w, h };

	renderer = new Renderer(wd);

	//input.AddKeyboardHandler(KEY_SPACE, pressed, []() -> void { DEBUG("{EQ"); });
	input.AddKeyboardHandler(KEY_ESCAPE, released, []() -> void { PostQuitMessage(0); });
}


Kernel::~Kernel()
{
	delete renderer;
}

long int _stdcall Kernel::WinMessage(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam)
{
	Kernel* kernel = 0;

	if (_message == WM_NCCREATE)
		kernel = (Kernel*)_lParam;
	else
		kernel = (Kernel*)GetWindowLong(_window, GWL_USERDATA);

	return kernel->Message(_window, _message, _wParam, _lParam);
}

long int Kernel::Message(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (_message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	/*case WM_MOUSEMOVE: 
	case WM_LBUTTONUP: 
	case WM_LBUTTONDOWN: 
	case WM_MBUTTONUP: 
	case WM_MBUTTONDOWN: 
	case WM_RBUTTONUP: 
	case WM_RBUTTONDOWN: 
	case WM_MOUSEWHEEL:*/ 
	case WM_KEYUP:
	case WM_KEYDOWN: 
		input.Run(_message, _wParam, _lParam);
		// TODO обработчик картой функций
		break;
	default:
		return DefWindowProc(_window, _message, _wParam, _lParam);
	}

	return 0;
}