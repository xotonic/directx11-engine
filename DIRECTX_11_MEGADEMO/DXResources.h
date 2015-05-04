#pragma once
#include "common.h"
#include "debug.h"
using namespace Microsoft::WRL;

class DXResources
{
public:
	DXResources(WindowDescriptor& wd);
	~DXResources();

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain>      swapChain;

	ComPtr<ID3D11RenderTargetView> renderTargetView ;
	ComPtr<ID3D11Texture2D>        depthTexture;         // Текстура буфера глубин
	ComPtr<ID3D11DepthStencilView> depthStencilView;     // Объект вида, буфер глубин

	//void AdaptToWindow(WindowDescriptor& wd);
	void Present();
	void ClearView();
	ID3D11Device* getDevice() { return device.Get(); }
private:
	//bool adapded;
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor;
	D3D_DRIVER_TYPE driverType;
};

