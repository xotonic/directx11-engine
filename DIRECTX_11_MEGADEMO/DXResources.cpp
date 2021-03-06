#include "DXResources.h"

DXResources::DXResources(WindowDescriptor& wd) : winDesc(wd) //: adapded(false)
{
	ZeroMemory(&swapChainDescriptor, sizeof(swapChainDescriptor));
	swapChainDescriptor.BufferCount = 1;
	swapChainDescriptor.BufferDesc.Width = wd.size.width;
	swapChainDescriptor.BufferDesc.Height = wd.size.height;
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescriptor.OutputWindow = wd.hWnd;
	swapChainDescriptor.SampleDesc.Count = 1;
	swapChainDescriptor.SampleDesc.Quality = 0;
	swapChainDescriptor.Windowed = TRUE;

	driverType = D3D_DRIVER_TYPE_HARDWARE;
	UINT flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL fl;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	HRESULT hr;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			flags, // device flags
			featureLevels, // array of feature levels
			numFeatureLevels, // num feature levels
			D3D11_SDK_VERSION,
			&swapChainDescriptor,
			&swapChain,
			&device,
			&fl,
			&deviceContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		Debug::message("CREATE DEVICE ERROR\n");
		return;
	}

	ID3D11Texture2D* pBackBuffer = NULL;
	Debug::if_failed(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
		"error getting back buffer");

	Debug::if_failed(device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView),
		"error in creating render target view");
	pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = wd.size.width;
	descDepth.Height = wd.size.height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	Debug::if_failed(device->CreateTexture2D(&descDepth, nullptr, &depthTexture),
		"Error creating depth texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         // ������ ��� � ��������
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	Debug::if_failed(device->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView),
		"Error creating depth stencil view");

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.Height = (float)wd.size.width;
	vp.Width = (float)wd.size.width;
	vp.MinDepth = 0.0;
	vp.MaxDepth = 1.0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);

	swapChain->GetBuffer(0, __uuidof(IDXGISurface2), (void**)&dxgiBackBuffer);

	Init2d();
}

DXResources::~DXResources()
{
	if (dwFactory) dwFactory->Release();
	if (depthStencilView) depthStencilView->Release();
	if (depthTexture) depthTexture->Release();
	if (renderTargetView) renderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (device) device->Release();
}

void DXResources::Present()
{
	Debug::if_failed(swapChain->Present(1, 0), "Present() error");
}

void DXResources::ClearView()
{
	float clearColor[4] = { 0.49f, 0.80f, 1.0f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
}

void DXResources::Init2d()
{
	Debug::if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory), "");

	FLOAT dpiX;
	FLOAT dpiY;
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
		);

	d2dFactory->CreateDxgiSurfaceRenderTarget(
		dxgiBackBuffer,
		&props,
		&d2dRT
		);

	Debug::if_failed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(dwFactory),
		reinterpret_cast<IUnknown **>(&dwFactory)),
		"error creating DirectWrite Factory"
		);
}