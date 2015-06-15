#pragma once
#include <sstream>
#include <Windows.h>

class Debug
{
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
#define MESSAGE(x) \
	do {\
	std::stringstream s; \
	s << (x); \
	MessageBox(NULL,s.str().c_str(),"Message",MB_OK); \
					} while (0)
/* функция, текст ошибки*/
#define CHECK_HRESULT( x , y ) {\
	HRESULT hr = S_OK;\
	hr = x;\
	if (FAILED(hr)) {\
std::stringstream msg; \
	msg << y << "\nError code : 0x" << std::hex << hr;\
MESSAGE(msg.str().c_str());}\
				}

