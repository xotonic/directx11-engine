#pragma once

#include <vector>
#include <string>
#include <map>
#include <list>
#include "common.h"
#include "DXResources.h"
#include "debug.h"

using namespace std;

class Console
{
public:
	Console(DXResources* _dx);
	
	void SetParam(string name, wstring value);
	void SetParam(string name, wstring caption, int value);
	void SetParam(string name, wstring caption, float value);
	void SetParam(string name, wstring caption, XMFLOAT3 xyz);

	void SetMessage(wstring value);
	
	void Update();
	
private:
	// параметры, которые будут выводиться всегда
	DXResources* dx;
	map<string, wstring> params;
	
	// сообщения, которые будут исчезать через
	// некоторое кол-во кадров
	list<wstring> messages;
	//queue<unsigned int> msg_times;
	const unsigned int message_life_time = 100;
	UINT t;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* brush;
};

