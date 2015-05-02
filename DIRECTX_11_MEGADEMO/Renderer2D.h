#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <windows.h>
#include "debug.h"

class Renderer2D
{
public:
	Renderer2D(HWND hwnd);
	~Renderer2D();

	void Render();
private:
	ID2D1Factory* _2dFactory;
	ID2D1HwndRenderTarget* _2dHwndRT;
	ID2D1RenderTarget* RT;
	ID2D1SolidColorBrush* brush;

	IDWriteFactory* dwFactory;
	IDWriteTextFormat* dwTextFormat;
	IDWriteTextLayout* dwTextLayout;
};

