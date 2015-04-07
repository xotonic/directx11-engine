#pragma once
#include "common.h"
class DeviceDependent
{
protected:
	DeviceDependent(ID3D11Device * dev);
	
	ID3D11Device* device= nullptr;
	ID3D11DeviceContext *deviceContext = nullptr;

	std::wstring stringToWstring(std::string str)
	{
		std::wstring temp = std::wstring(str.begin(), str.end());
		return temp;
		/*int len;
		int slength = (int)str.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;*/
	}
};

