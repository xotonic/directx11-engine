#include "Renderer.h"

Renderer::Renderer(DXResources* dx) : angle(0)
{
	camera.InitProjMatrix(XM_PIDIV2, dx->winDesc.size.width, dx->winDesc.size.height, 0.01f, 1000.0f);

	world = XMMatrixIdentity();
	view = camera.View();
	projection = camera.Proj();

	resMgr = new ResourceManager(dx->getDevice());

	terrain = new Terrain(dx->getDevice(), "out.terrain");
	resMgr->readResources("resources.txt");

	matrices = new ConstantBuffer < MatrixBuffer >(dx->getDevice());
	light = new ConstantBuffer<DirLightBuffer>(dx->getDevice());

	light->data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->data.dir = XMFLOAT4(-0.577f, 0.0f, -0.5f, 1.0f);

	light->update();
	light->bind_PS(0);

	lines = std::make_shared<Line>(dx);

	ent = new Entity(resMgr, "monkey", "default", "grass", "grass_normal");
	ent->transform()->Move({1.0,1.0,1.0});
}

Renderer::~Renderer()
{
	delete matrices;
	delete light;
	delete terrain;
	delete resMgr;
	delete ent;

}

void Renderer::Render()
{

	resMgr->shader("default")->bind();
	resMgr->mesh("monkey")->bind();

	light->update();
	light->bind_PS(0);

	matrices->data.world = transform.World();//XMMatrixTranspose(world);
	matrices->data.view = camera.View();// XMMatrixTranspose(view);
	matrices->data.projection = projection;

	matrices->update();
	matrices->bind_VS(0);

	resMgr->texture("grass")->bind(0);
	resMgr->texture("grass_normal")->bind(1);

	resMgr->mesh("monkey")->draw();

	terrain->bind();

	matrices->data.world = tr.World();
	matrices->update();
	matrices->bind_VS(0);

	terrain->draw();
	matrices->data.world = ent->transform()->World();
	matrices->update();
	matrices->bind_VS(0);

	ent->Draw();
	lines->Draw(camera);
}