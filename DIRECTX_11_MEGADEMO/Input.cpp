#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::AddKeyboardHandler(KeyCode code, KeyState keyEvent, std::function<void()> func)
{
	KeyEvent e(code, keyEvent);

	auto it = keys.find(e);

	if (it != keys.end())
		keys[e].push_back(func);
	else
	{
		std::vector<std::function<void()>> v;
		v.push_back(func);
		keys.insert(std::pair<KeyEvent, std::vector<std::function<void()>>>(e, v));
	}
}
void Input::Run(unsigned int nMsg, WPARAM wParam, LPARAM lParam)
{

	KeyCode code = static_cast<KeyCode>(wParam);
	KeyState state = (nMsg == WM_KEYDOWN) ? pressed : released;
	KeyEvent e = { code, state };

	auto it = keys.find(e);

	//if ((it != keys.end()) & ((*it).first == e))
	if(it!=keys.end()) //if ((*it).first == e)
	{
		std::vector<std::function<void()>> v = keys[e];
		for (auto iterator : v)
		{
			iterator();
		}
		//v.clear();
	}
}