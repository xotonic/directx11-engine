#pragma once
#include <sstream>
#include <Windows.h>

/*вывод в visual studio*/
#define DEBUG(x)\
	do {\
	std::stringstream s; \
	s << (x);\
	OutputDebugString(s.str().c_str()); \
			} while (0)

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
	msg << y << "\nError code : " << hr;\
MESSAGE(msg.str().c_str());}\
		}

/**/
// Helper utilities to make Win32 APIs work with exceptions.

/*	inline void ThrowIfFailed(HRESULT hr)
	{
	if (FAILED(hr))
	{
	// Set a breakpoint on this line to catch Win32 API errors.
	throw ::Platform::Exception::CreateException(hr);
	}
	}*/