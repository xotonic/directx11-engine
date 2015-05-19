#include "Renderer.h"

Renderer::Renderer(DXResources* dx) : angle(0)
{
	camera.InitProjMatrix(XM_PIDIV2, dx->winDesc.size.width, dx->winDesc.size.height, 0.01f, 1000.0f);

	world = XMMatrixIdentity();
	view = camera.View();
	projection = camera.Proj();

	resMgr = new ResourceManager(dx->getDevice());

	terrain = new Terrain(dx->getDevice(), "out.terrain");

	resMgr->loadShader("simple", "shader.vsh", "shader.psh", false, true, true);
	resMgr->loadMesh("cube", "mon.mesh");
	resMgr->loadTexture("diffuse", "grass.dds");
	resMgr->loadTexture("normal", "grass_normal.dds");

	resMgr->mesh("cube")->bind();
	resMgr->shader("simple")->bind();


	//camera.Move(0.0f, 1.0, -5.0);

	matrices = new ConstantBuffer < MatrixBuffer >(dx->getDevice());
	light = new ConstantBuffer<DirLightBuffer>(dx->getDevice());

	light->data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->data.dir = XMFLOAT4(-0.577f, 0.0f, -0.5f, 1.0f);

	light->update();
	light->bind_PS(0);

	lines = std::make_shared<Line>(dx);
	//lines->SetLine(XMVectorSet(0, 2, 0, 0), XMVectorSet(20, 20, 20, 0));
//	tr.Translate(4.0f, 4.0, 0.0);
}

Renderer::~Renderer()
{
	delete matrices;
	delete light;
	delete terrain;
	delete resMgr;

}

void Renderer::Render()
{
	/*float clearColor[4] = { 0.30f, 0.30f, 0.30f, 1.0f };

	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
	*/
	resMgr->shader("simple")->bind();

	static float t = 0.0f;
	t += 0.0005;

	resMgr->mesh("cube")->bind();

	light->update();
	light->bind_PS(0);

	matrices->data.world = transform.World();//XMMatrixTranspose(world);
	matrices->data.view = camera.View();// XMMatrixTranspose(view);
	matrices->data.projection = projection;

	matrices->update();
	matrices->bind_VS(0);

	resMgr->texture("diffuse")->bind(0);
	resMgr->texture("normal")->bind(1);

	//deviceContext->PSSetShaderResources(0, 1, &textureRV);
	//deviceContext->PSSetSamplers(0, 0, 0);
	resMgr->mesh("cube")->draw();



	matrices->data.world = tr.World();//XMMatrixTranspose(world);
	matrices->data.view = camera.View();// XMMatrixTranspose(view);
	matrices->data.projection = projection;


	matrices->update();
	matrices->bind_VS(0);
	light->bind_PS(0);


	//light->update();

	//light->update();
	//light->bind(1);
	resMgr->mesh("cube")->draw();

	terrain->bind();

	matrices->data.world = tr.World();
	matrices->update();
	matrices->bind_VS(0);

	//lines->basic_shader->bind();

	terrain->draw();
	lines->Draw(camera);
//	deviceContext->Draw(resMgr->mesh("cube")->getCount(), 0);

	//swapChain->Present(0, 0);
}