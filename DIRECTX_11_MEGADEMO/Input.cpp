#include "Input.h"


Input::Input()
{
}


Input::~Input()
{
}

void Input::AddKeyboardHandler(KeyCode code, KeyState keyEvent, void(*func)())
{
	KeyEvent e(code, keyEvent);
	
	auto it = keys.find(e);

	if (it != keys.end())
		keys[e].push_back(func);
	else 
	{
	std::vector<void(*)()> v;
	v.push_back(func);
	keys.insert(std::pair<KeyEvent,std::vector<void(*)()>>(e,v));
	
	}

}
void Input::Run(unsigned int nMsg, WPARAM wParam, LPARAM lParam)
{
	KeyCode code = static_cast<KeyCode>(wParam);
	KeyState state = (nMsg == WM_KEYDOWN) ? pressed : released;
	KeyEvent e = { code, state };

	auto it = keys.find(e);



	//if ((it != keys.end()) & ((*it).first == e))
	if ((*it).first == e)
	{
		std::vector<void(*)()> v = keys[e];
		for (auto iterator = v.begin(); iterator != v.end(); iterator++)
		{
			(*iterator)();
		}
		//v.clear();
	}
}
