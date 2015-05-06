#pragma once
#include <memory>

#include "common.h"
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

class Renderer
{
public:
	Renderer(DXResources* dx);
	~Renderer();
	void Render();

	/* Для XNA math нужно выравнивание на 16 байтов */
	/*void* operator new(size_t i)
	{
	return _aligned_malloc(i, 16);
	}

	void operator delete(void* p)
	{
	_aligned_free(p);
	}*/
	Transformable transform;
	Transformable tr;
	Camera camera;
	ResourceManager *resMgr;
	Terrain* terrain;
	ConstantBuffer<DirLightBuffer>* light;
private:

	VertexBuffer   *buf;
	Texture        *tex, *normal;
	//IndexBuffer  *ibuf;

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	float angle;

	ConstantBuffer<MatrixBuffer>* matrices;
};
