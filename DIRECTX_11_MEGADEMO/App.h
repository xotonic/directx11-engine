#pragma once

#include "stdafx.h"
#include "Kernel.h"
#include "Tank.h"
class App :
	public Kernel
{
public:
	App(HINSTANCE hInst, int nCmdShow, int w = 640, int h = 480) : Kernel(hInst, nCmdShow, w, h){ tank = new Tank(get_renderer());
	e = new Entity(get_renderer().resMgr, "mark", "default", "empty_diffuse", "empty_normal");
	};

	void OnRender(Renderer& renderer);
	void OnInputInitialize(Input& input);
	void OnRenderInitialize(Renderer& renderer);
	
	Tank* tank;
	Entity *e;
	~App();
};

