#pragma once
#include "common.h"
#include "debug.h"

//using namespace Microsoft::WRL;

class DeviceResources
{
public:
	DeviceResources();
	~DeviceResources();
	// Обьекты Direct3d
	ID3D11Device2         * device          ;
	ID3D11DeviceContext2  * devcon          ;
	IDXGISwapChain1       * swapChain       ;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	//ID2D1Factory2         * d2dFactory2      ;
	IDWriteFactory2       * dwriteFactory   ;
	IWICImagingFactory2   * wicFactory      ;
	
	ID3D11Device       * dev;
	ID3D11DeviceContext* context;
	ID3D11Texture2D    * depthStencil;


	IDXGIDevice3       * dxgiDevice;
	ID2D1DeviceContext    * d2dDevcon       ;
	ID2D1Device1          * d2dDevice       ;
	ID3D11Texture2D    * backBuffer;
	ID2D1Bitmap1          * d2dTargetBitmap ;
	ID2D1Factory2         * d2dFactory;
	IDXGISurface2      *   dxgiBackBuffer;

	D3D11_VIEWPORT		viewport;
	D3D_FEATURE_LEVEL featureLevel;
	
	WindowDescriptor winDesc;
	void AdaptToWindow(WindowDescriptor wd);
	void Present();
	void ClearView();
	ID3D11Device* getDevice() { return dev; }
private:
	void CreateFactories();
	void CreateDeviceResources();
#if defined(_DEBUG)
	// Проверьте поддержку уровней пакета SDK.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // Нет необходимости создавать настоящее физическое устройство.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Проверьте уровни пакета SDK.
			nullptr,                    // Годится любой функциональный уровень.
			0,
			D3D11_SDK_VERSION,          // Всегда устанавливать равным D3D11_SDK_VERSION для приложений для Магазина Windows.
			nullptr,                    // Нет необходимости сохранять ссылку на устройство D3D.
			nullptr,                    // Нет необходимости знать функциональный уровень.
			nullptr                     // Нет необходимости сохранять ссылку на контекст устройства D3D.
			);

		return SUCCEEDED(hr);
	}
#endif
};

