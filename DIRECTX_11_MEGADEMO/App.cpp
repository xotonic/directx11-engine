#include "App.h"


void App::OnRender(Renderer& renderer)
{
	tank->Draw(&renderer);
	renderer.DrawEntity(e);
}

void App::OnInputInitialize(Input& input)
{

}

void App::OnRenderInitialize(Renderer &renderer)
{
}

App::~App()
{
	delete tank;
}
