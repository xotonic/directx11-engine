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
			timer.reset();
			timer.doBefore();

			//renderer2D->console->SetParam("cam", L"camera", renderer->camera.getPos());
			input->Update();
			dx->ClearView();
			renderer->Render();
			renderer2D->Render();
			dx->Present();

			timer.doAfter();

		}
	}
}

Kernel::Kernel(HINSTANCE hInst, int nCmdShow, int w, int h) //: input(w,h)
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

	int y_max = GetSystemMetrics(SM_CYSCREEN);
	int x_max = GetSystemMetrics(SM_CXSCREEN);

	int x_pos = (x_max - w) / 2;
	int y_pos = (y_max - h) / 2;

	hInstance = hInst;
	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"Копатели Онлайн",            // title-bar string
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // top-level window
		x_pos,       // default horizontal position
		y_pos,       // default vertical position
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

	input = std::make_shared<Input>(hInstance, hWnd, w, h);
	dx = std::make_shared<DXResources>(wd);
	renderer = std::make_shared<Renderer>(dx.get());
	renderer2D = std::make_shared<Renderer2D>(dx.get());

	Renderer* r = renderer.get();
	Renderer2D* r2d = renderer2D.get();
	Timer* t = &timer;
	HWND hwnd = hWnd;

	r->camera.Move({0,25,0});
	r->camera.Rotate({ 1, 0, 0 }, 60);
	
	r->light->data.dir = {0.12f, 0.42f, -0.81f, 1.0f};

	input->AddKeyboardHandler(KEY_D, pressed, [r]() -> void { r->camera.Move({0.5f, 0.f, 0.f });});
	input->AddKeyboardHandler(KEY_W, pressed, [r]() -> void { r->camera.Move({0.0f, 0.f, 0.5f});});
	input->AddKeyboardHandler(KEY_A, pressed, [r]() -> void { r->camera.Move({-0.5f, 0.f, 0.f});});
	input->AddKeyboardHandler(KEY_S, pressed, [r]() -> void { r->camera.Move({0.f, 0.f, -0.5f});});
	
	input->AddKeyboardHandler(KEY_LEFT, pressed, [r]() -> void { r->transform.Move({ -0.5f, 0.f, 0.f }); });
	input->AddKeyboardHandler(KEY_UP, pressed, [r]() -> void { r->transform.Move({ 0.f, 0.f, 0.5f }); });
	input->AddKeyboardHandler(KEY_RIGHT, pressed, [r]() -> void { r->transform.Move({ 0.5f, 0.f, 0.f }); });
	input->AddKeyboardHandler(KEY_DOWN, pressed, [r]() -> void { r->transform.Move({ 0.f, 0.f, -0.5f }); });


	input->AddKeyboardHandler(KEY_1, pressed, [r, r2d]() -> void { 
		/*XMVECTOR v = XMLoadFloat4(&r->light->data.dir);
		v = XMVector3Rotate(v, XMVectorSet( 0.01f, 0.0f, 0.0f, 1.0f ));
		XMStoreFloat4(&r->light->data.dir, v);
*/

	});

	input->AddKeyboardHandler(KEY_2, pressed, [r]() -> void {
		XMVECTOR v = XMLoadFloat4(&r->light->data.dir);
		v = XMVector3Rotate(v, XMVectorSet(0.0f, 0.01f, 0.0f, 1.0f));
		XMStoreFloat4(&r->light->data.dir, v);
	});

	input->AddKeyboardHandler(KEY_3, pressed, [r]() -> void {
		XMVECTOR v = XMLoadFloat4(&r->light->data.dir);
		v = XMVector3Rotate(v, XMVectorSet(0.0f, 0.0f, 0.01f, 1.0f));
		XMStoreFloat4(&r->light->data.dir, v);
	});

	input->AddKeyboardHandler(KEY_ESCAPE, released, []() -> void { PostQuitMessage(0); });

	input->AddMouseHandler(MOUSE_LEFT, released, [r, r2d, hwnd](const  int x, const  int y) -> void
	{  
		VectorPair ray = r->camera.getRay(x, y);
		r->lines->SetLine(ray);
		
		XMVECTOR plane = XMPlaneFromPoints(XMVectorSet(1, 0, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(1, 0, 1, 0));
		XMVECTOR pos = XMPlaneIntersectLine(plane, ray.first, ray.second);
		r->transform.Position(to(pos));
	});

	input->AddMouseMoveHandler([r2d](const int dx, const int dy, const int x, const int y) -> void
	{ 
		//r->camera.Rotate({ 0, 1, 0 }, dx*0.1); r->camera.Rotate({ 1, 0, 0 }, dy*0.1); 
		wostringstream os; os << L"dx : " << dx << L" dy : " << dy << endl
			                  << L"x : " << x << L" y : " << y << endl;
		r2d->console->SetParam("mouse", os.str());

	}
	);

	timer.addAfterHandler("fps", [t, r2d, r]() -> void 
	{ 
		r2d->console->SetParam("fps", L"FPS: ", 1000/(!t->getDeltaTime() ? 1 : t->getDeltaTime()));
		r2d->console->SetParam("cam", L"camera position", r->camera.Position());
		r2d->console->SetParam("ldir", L"light dir", to(r->light->data.dir));

		//r->lines->SetLine(XMVectorZero(), to(r->transform.Position()));
	});

	//r2d->console->SetParam("render proj",L"из renderer", r->projection);
	//r2d->console->SetParam("cam proj", L"из camera", to(r->camera.Proj()));

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

	//	/*case WM_MOUSEMOVE:
	//	case WM_LBUTTONUP:*/
	//	//case WM_LBUTTONDOWN:
	//	/*case WM_MBUTTONDOWN:
	//	case WM_RBUTTONUP:
	//	case WM_RBUTTONDOWN:
	//	case WM_MOUSEWHEEL:*/
	//	//case WM_MOUSEMOVE:
	//		//input.RunMouse(_wParam, _lParam, MOUSE_NONE);
	//		//SetCapture(hWnd);
	//		//SetCursorPos(640, 480);
	//		//break;
	//	//case WM_LBUTTONUP:
	//		//input.RunMouse(_wParam, _lParam, MOUSE_LEFT, released);
	//		//break;
	////case WM_KEYUP:
	////case WM_KEYDOWN:
	//	//input.Run(_message, _wParam, _lParam);
	//	// TODO обработчик картой функций
	//	//break;
		
	default:
		return DefWindowProc(_window, _message, _wParam, _lParam);
	}

	return 0;
}