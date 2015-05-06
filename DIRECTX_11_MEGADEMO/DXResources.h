#pragma once
#include "common.h"
#include "debug.h"

class DXResources
{
public:
	DXResources(WindowDescriptor& wd);
	~DXResources();
	
	//Основные обьекты и обьекты для Direct3D
	ID3D11Device          *        device           ;
	ID3D11DeviceContext   *        deviceContext    ;
	IDXGISwapChain        *        swapChain        ;
	ID3D11RenderTargetView*        renderTargetView ;
	ID3D11Texture2D       *        depthTexture     ;         // Текстура буфера глубин
	ID3D11DepthStencilView*        depthStencilView ;     // Объект вида, буфер глубин

	WindowDescriptor winDesc;

	ID3D11Device* getDevice() { return device; }
	void Present();
	void ClearView();

	// Обьекты для Direct2D
	ID2D1Factory         * d2dFactory;
	IDXGISurface      *   dxgiBackBuffer;
	ID2D1RenderTarget* d2dRT;

	// Обьекты для DirectWrite
	IDWriteFactory* dwFactory;
private:
	void Init2d();
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor;
	D3D_DRIVER_TYPE driverType;
};

