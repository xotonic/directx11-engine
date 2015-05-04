#pragma once
#include "common.h"
#include "debug.h"
//using namespace Microsoft::WRL;

class DXResources
{
public:
	DXResources(WindowDescriptor& wd);
	~DXResources();

	ID3D11Device          *        device           ;
	ID3D11DeviceContext   *        deviceContext    ;
	IDXGISwapChain        *        swapChain        ;
	ID3D11RenderTargetView*        renderTargetView ;
	ID3D11Texture2D       *        depthTexture     ;         // Текстура буфера глубин
	ID3D11DepthStencilView*        depthStencilView ;     // Объект вида, буфер глубин

	WindowDescriptor winDesc;

	//void AdaptToWindow(WindowDescriptor& wd);
	void Present();
	void ClearView();
	ID3D11Device* getDevice() { return device; }

	/////////////////
	ID2D1Factory         * d2dFactory;
	IDXGISurface      *   dxgiBackBuffer;
	ID2D1RenderTarget* d2dRT;

	ID2D1SolidColorBrush    *pBrush;
	D2D1_ELLIPSE            ellipse;

	void Init2d();
private:
	//bool adapded;
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor;
	D3D_DRIVER_TYPE driverType;
};

