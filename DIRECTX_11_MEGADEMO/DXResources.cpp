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
		MESSAGE("CREATE DEVICE ERROR\n");
		return;
	}

	ID3D11Texture2D* pBackBuffer = NULL;
	CHECK_HRESULT(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
		"error getting back buffer");
	

	CHECK_HRESULT(device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView),
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

	CHECK_HRESULT(device->CreateTexture2D(&descDepth, nullptr, &depthTexture),
		"Error creating depth texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         // формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	CHECK_HRESULT(device->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView),
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
	if (depthStencilView) depthStencilView->Release();
	if (depthTexture) depthTexture->Release();
	if (renderTargetView) renderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (device) device->Release();

}

/*void DXResources::AdaptToWindow(WindowDescriptor& wd)
{}*/

void DXResources::Present()
{
	d2dRT->BeginDraw();
	d2dRT->FillEllipse(ellipse, pBrush);
	d2dRT->EndDraw();

	CHECK_HRESULT(swapChain->Present(1, 0), "Present() error");
}

void DXResources::ClearView()
{
	float clearColor[4] = { 0.30f, 0.30f, 0.30f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	//swapChain->GetBuffer(0, __uuidof(IDXGISurface2), (void**)&dxgiBackBuffer);
}

void DXResources::Init2d()
{
	
	CHECK_HRESULT(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory), "");
	
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

	if (!d2dFactory) MESSAGE("2d factory is null");

	d2dFactory->CreateDxgiSurfaceRenderTarget(
		dxgiBackBuffer,
		&props,
		&d2dRT
		);
	
	const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
	 d2dRT->CreateSolidColorBrush(color, &pBrush);
	 ellipse = D2D1::Ellipse(D2D1::Point2F(800.0f, 250.0f), 20, 20);
}
