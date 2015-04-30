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
//#include "IndexBuffer.h"
#include "Terrain.h"
#include "debug.h"

class Renderer
{
public:
	Renderer(WindowDescriptor wd);
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

	D3D_DRIVER_TYPE driverType;

	ID3D11Device*        device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain*      swapChain;

	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11Texture2D*        depthTexture = nullptr;         // Текстура буфера глубин
	ID3D11DepthStencilView* depthStencilView = nullptr;     // Объект вида, буфер глубин

	//ID3D11ShaderResourceView *textureRV = nullptr;
	//ID3D11SamplerState *sampler = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDescriptor;

	ID3D11RasterizerState* noCullingState;
	void createDevice(WindowDescriptor wd);

	//Shader         *shader;
	VertexBuffer   *buf;
	Texture        *tex, *normal;
	//IndexBuffer  *ibuf;

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	float angle;

	ConstantBuffer<MatrixBuffer>* matrices;
};
