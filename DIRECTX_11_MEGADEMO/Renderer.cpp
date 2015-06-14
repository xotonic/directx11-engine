#include "Renderer.h"

Renderer::Renderer(DXResources* _dx) : angle(0), dx(_dx)
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

	lines = std::make_shared<Line>(dx,5);
	camera_ray = std::make_shared<Line>(dx, 2);
	ent = new Entity(resMgr, "mark", "default", "empty_diffuse", "empty_normal");
	ent->transform()->Move({10.0f,0.1f,1.0f});
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
	resMgr->mesh("head")->bind();

	light->update();
	light->bind_PS(0);

	matrices->data.world = transform.World();//XMMatrixTranspose(world);
	matrices->data.view = camera.View();// XMMatrixTranspose(view);
	matrices->data.projection = projection;

	matrices->update();
	matrices->bind_VS(0);

	resMgr->texture("grass")->bind(0);
	resMgr->texture("grass_normal")->bind(1);

	resMgr->mesh("head")->draw();

	terrain->bind();

	matrices->data.world = XMMatrixTranspose(XMMatrixIdentity());
	matrices->update();
	matrices->bind_VS(0);

	terrain->draw();
	matrices->data.world = ent->transform()->World();
	matrices->update();
	matrices->bind_VS(0);

	//ent->transform()->Scale(0.9f);
	ent->Draw();

	//for (auto line : lines)
	lines->Draw(camera);
	camera_ray->Draw(camera);
}