#pragma once
#include "common.h"
#include "debug.h"

using namespace Microsoft::WRL;

class DeviceResources
{
public:
	DeviceResources();
	// Обьекты Direct3d
	ComPtr<ID3D11Device2> device;
	ComPtr<ID3D11DeviceContext2> devcon;
	ComPtr<IDXGISwapChain1> swapChain;
	// Обьекты для трехмерной отрисовки
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	// Обьекты Direct2d
	ComPtr<ID2D1Factory2> d2dFactory;
	ComPtr<ID2D1Device1> d2dDevice;
	ComPtr<ID2D1DeviceContext> d2dDevcon;
	ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	// Обьекты DirectWrite
	ComPtr<IDWriteFactory2> dwriteFactory;
	ComPtr<IWICImagingFactory2> wicFactory;

	D3D_FEATURE_LEVEL featureLevel;
	void AdaptToWindow(WindowDescriptor wd);
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

