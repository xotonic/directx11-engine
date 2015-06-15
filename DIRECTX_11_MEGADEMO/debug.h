#pragma once
#include <sstream>
#include <windows.h>

class Debug
{
public:
	static void message(std::string str)
	{
				MessageBox(NULL, str.c_str(), "Message", MB_OK); 
	}
	static void if_failed(HRESULT hr, std::string str)
	{
		if (FAILED(hr)) {
			
				std::stringstream msg; 
				msg << str << "\nError code : 0x" << std::hex << hr; 
				message(msg.str().c_str());
		}
	}
};

