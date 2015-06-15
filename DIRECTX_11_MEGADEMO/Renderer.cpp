#include "Renderer.h"

Renderer::Renderer(DXResources* _dx) : angle(0), dx(_dx)
{
	camera.InitProjMatrix(XM_PIDIV2, dx->winDesc.size.width, dx->winDesc.size.height, 0.01f, 1000.0f);

	world = XMMatrixIdentity();
	view = camera.View();
	projection = camera.Proj();

	resMgr = new ResourceManager(dx->getDevice());

	SetWindowTextW(dx->winDesc.hWnd, L"Загрузка ландшафта...");
	terrain = new Terrain(dx->getDevice(), "out.terrain");
	SetWindowTextW(dx->winDesc.hWnd, L"Загрузка объектов...");
	resMgr->readResources("resources.txt");

	matrices = new ConstantBuffer < MatrixBuffer >(dx->getDevice());
	light = new ConstantBuffer<DirLightBuffer>(dx->getDevice());

	light->data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->data.dir = XMFLOAT4(-0.577f, 0.0f, -0.5f, 1.0f);

	light->update();
	light->bind_PS(0);

	lines = std::make_shared<Line>(dx, 5);
	camera_ray = std::make_shared<Line>(dx, 2);
	ent = new Entity(resMgr, "mark", "default", "empty_diffuse", "empty_normal");
	tree = new Entity(resMgr, "tree", "default", "bark", "bark_normal");
	ent->transform()->Move({ 10.0f, 0.1f, 1.0f });

	player = new Player(resMgr);
	SetWindowTextW(dx->winDesc.hWnd, L"Генерация сцены...");
	GenerateObjects();
	SetWindowTextW(dx->winDesc.hWnd, L"~ Kursach - Benchmark. Mangal Edition ~");
}

Renderer::~Renderer()
{
	delete player;
	delete matrices;
	delete light;
	delete terrain;
	delete resMgr;
	delete ent;
}

void Renderer::Render()
{
	light->update();
	light->bind_PS(0);

	matrices->data.view = camera.View();
	matrices->data.projection = projection;

	DrawTerrain();
	DrawObjects();
	DrawEntity(ent);
	DrawEntity(player->head);
	DrawEntity(player->body);

	lines->Draw(camera);
	camera_ray->Draw(camera);
}

void Renderer::DrawEntity(Entity* e)
{
	matrices->data.world = e->transform()->World();
	matrices->update();
	matrices->bind_VS(0);
	e->Draw();
}

void Renderer::GenerateObjects()
{
	for (int i = 0; i < 1000; i++)
	{
		float x = std::rand() % (terrain->wight - 1);
		float z = std::rand() % (terrain->height - 1);
		XMVECTOR origin = XMVectorSet(x, 100.0f, z, 0.0f);
		XMVECTOR dir = XMVectorSet(x, -1.0f, z, 0.0f);
		float y = terrain->getQuad(x, z).maxHeigth();
		XMMATRIX m = XMMatrixIdentity();
		m *= XMMatrixRotationRollPitchYaw(-XM_PIDIV2, 0, (std::rand() % 10));
		m *= XMMatrixTranslation(x, y, z);
		stones.push_back(m);
	}

	for (int i = 0; i < 100; i++)
	{
		float x = std::rand() % (terrain->wight - 1);
		float z = std::rand() % (terrain->height - 1);
		XMVECTOR origin = XMVectorSet(x, 100.0f, z, 0.0f);
		XMVECTOR dir = XMVectorSet(x, -1.0f, z, 0.0f);
		float y = terrain->getQuad(x, z).maxHeigth();
		XMMATRIX m = XMMatrixIdentity();
		//m *= XMMatrixRotationRollPitchYaw(-XM_PIDIV2, 0, (std::rand() % 10));
		m *= XMMatrixTranslation(x, y, z);
		forest.push_back(m);
	}
}

void Renderer::DrawObjects()
{
	for (auto m : stones)
	{
		matrices->data.world = XMMatrixTranspose(m);
		matrices->update();
		matrices->bind_VS(0);

		ent->Draw();
	}

	for (auto m : forest)
	{
		matrices->data.world = XMMatrixTranspose(m);
		matrices->update();
		matrices->bind_VS(0);

		tree->Draw();
	}
}

void Renderer::DrawTerrain()
{
	resMgr->shader("default")->bind();
	resMgr->texture("grass")->bind(0);
	resMgr->texture("grass_normal")->bind(1);

	terrain->bind();

	matrices->data.world = XMMatrixTranspose(XMMatrixIdentity());
	matrices->update();
	matrices->bind_VS(0);

	terrain->draw();
}