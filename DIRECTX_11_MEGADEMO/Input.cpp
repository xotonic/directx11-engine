#include "Input.h"

Input::Input(HINSTANCE hinst, HWND _hwnd, int w, int h) : hwnd(_hwnd),
width(w), height(h),
x(0), y(0)
{
	CHECK_HRESULT(
		DirectInput8Create(hinst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		NULL),
		"Error creating DirectInput main class"
		);

	/* keyboard */

	CHECK_HRESULT(
		directInput->CreateDevice(GUID_SysKeyboard, &keyboard, 0),
		"Error creating keyboard device");

	CHECK_HRESULT(
		keyboard->SetDataFormat(&c_dfDIKeyboard),
		"Error setting data format for keyboard device"
		);
	CHECK_HRESULT(
		keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE),
		"Error setting cooperative level for keyboard device"
		);

	CHECK_HRESULT(
		keyboard->Acquire(),
		"Error acquire keyboard"
		);

	/* mouse */

	CHECK_HRESULT(
		directInput->CreateDevice(GUID_SysMouse, &mouse, 0),
		"Error creating mouse device"
		);

	CHECK_HRESULT(
		mouse->SetDataFormat(&c_dfDIMouse),
		"Error setting data format for mouse device"
		);

	CHECK_HRESULT(
		mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE),
		"Error setting cooperative level for mouse device"
		);

	CHECK_HRESULT(
		mouse->Acquire(),
		"Error acquire mouse"
		);
}

Input::~Input()
{
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
	}

	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
	}

	if (directInput) directInput->Release();
}

void Input::AddKeyboardHandler(KeyCode code, KeyState keyEvent, func_void func)
{
	KeyEvent e(code, keyEvent);

	auto it = keys.find(e);

	if (it != keys.end())
		keys[e].push_back(func);
	else
	{
		func_vector v;
		v.push_back(func);
		keys.insert(std::pair<KeyEvent, func_vector>(e, v));
	}
}

void Input::AddMouseHandler(MouseKeyCode code, KeyState state, func_coords func)
{
	if (code == MOUSE_LEFT)
	{
		if (state == pressed)
		{
			left_button_down_funcs.push_back(func);
		}
		else if (state == released)
		{
			left_button_up_funcs.push_back(func);
		}
	}
}

void Input::AddMouseMoveHandler(func_move func)
{
	move_funcs.push_back(func);
}

void Input::Update()
{
	ReadKeyboard();
	ReadMouse();
	ProcessInput();

	for (auto key : keys)
	{
		if (CheckKey(key.first.code) == pressed)
		{
			for (auto func : key.second)
			{
				func();
			}
		}
	}

	for (auto it : move_funcs)
		it(dx, dy, x, y);
	if (mouseState.rgbButtons[0] & 0x80)
	{
		for (auto it : left_button_down_funcs)
			it(x, y);
		for (auto it : left_button_up_funcs)
			it(x, y);
	}
}

void Input::ReadKeyboard()
{
	HRESULT hr = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
		else MESSAGE("error getting keyboard device state");
	}
}

void Input::ReadMouse()
{
	HRESULT hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else MESSAGE("error getting mouse device state");
	}
}

void Input::ProcessInput()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	x = point.x;
	y = point.y;

	dx = mouseState.lX;
	dy = mouseState.lY;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	if (x > width) x = width;
	if (y > height) y = height;
}

KeyState Input::CheckKey(KeyCode state)
{
	if (keyboardState[state] & 0x80) return pressed;
	else return released;
}

//void Input::Run(unsigned int nMsg, WPARAM wParam, LPARAM lParam)
//{
//
//	KeyCode code = static_cast<KeyCode>(wParam);
//	KeyState state = (nMsg == WM_KEYDOWN) ? pressed : released;
//	KeyEvent e = { code, state };
//
//	auto it = keys.find(e);
//
//	//if ((it != keys.end()) & ((*it).first == e))
//	if(it!=keys.end()) //if ((*it).first == e)
//	{
//		func_vector v = keys[e];
//		for (auto iterator : v)
//		{
//			iterator();
//		}
//		//v.clear();
//	}
//}
//
//void Input::RunMouse(WPARAM wParam, LPARAM lParam, MouseKeyCode mkc, KeyState ks)
//{
//	unsigned int new_x = GET_X_LPARAM(lParam),
//		new_y = GET_Y_LPARAM(lParam);
//
//	//std::ostringstream i;
//	//i << "x = " << new_x << " y = " << new_y;
//	//MESSAGE(i.str());
//
//	/* apply moving mouse actions */
//
//	for (auto it : move_funcs)
//		it(x - new_x, y - new_y);
//
//
//	/* apply click actions */
//	if (mkc == MOUSE_LEFT)
//		if (ks == released)
//		for (auto it : left_button_up_funcs)
//			it(new_x, new_y);
//
//	x = new_x; y = new_y;
//
//	//SetCursorPos(width/2, height/2);
//}