#pragma once
#include <memory>

#include "stdafx.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Transformable.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "DXResources.h"
//#include "IndexBuffer.h"
#include "Terrain.h"
#include "debug.h"
#include "Line.h"
#include "Entity.h"
#include "Player.h"
#include <vector>
class Renderer
{
public:
	Renderer(DXResources* _dx);
	~Renderer();
	void Render();

	Transformable transform;
	Transformable tr;
	Camera camera;
	ResourceManager *resMgr;
	Terrain* terrain;
	ConstantBuffer<DirLightBuffer>* light;
	XMMATRIX projection;

	Entity* ent;
	Player* player;
	std::shared_ptr<Line> camera_ray;
	std::shared_ptr<Line> lines;
	void DrawEntity(Entity* e);
	void DrawTerrain();
private:

	void DrawLines();
	DXResources* dx;
	VertexBuffer   *buf;
	Texture        *tex, *normal;
	//IndexBuffer  *ibuf;

	XMMATRIX world;
	XMMATRIX view;

	float angle;

	ConstantBuffer<MatrixBuffer>* matrices;
};
