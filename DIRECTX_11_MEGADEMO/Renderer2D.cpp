#include "Renderer2D.h"

Renderer2D::Renderer2D(DXResources* _dx): dx(_dx)
{
	console = new Console(dx);

	console->SetParam("param1", L"new param");
}

void Renderer2D::Render()
{
	dx->d2dRT->BeginDraw();
	console->Update();
	dx->d2dRT->EndDraw();
}

