#include "DeviceResources.h"


DeviceResources::DeviceResources()
{
	CreateFactories();
	CreateDeviceResources();
}


void DeviceResources::CreateFactories()
{
	// ������������� �������� Direct2D.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// ���� ������ �� �������� ���������� �������, �������� ������� Direct2D � ������� ������� SDK.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// ������������� ������� Direct2D.
	CHECK_HRESULT(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		&d2dFactory),
		"error creating Direct2d factory"
		);

	// ������������� ������� DirectWrite.
	CHECK_HRESULT(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&dwriteFactory),
		"error creating DirectWrite factory"
		);

	// ������������� ������� ���������� ��������� ����������� Windows (WIC).
	    CHECK_HRESULT(
		CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory)),
		"error creating WIC factory"
		);

}

void DeviceResources::CreateDeviceResources()
{
	// ���� ���� ��������� ��������� ������������ � ������ �������� �������� �������
	// �� ��������� � API �� ���������. �� �������� ������������ ��� ������������� � Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		// ���� ������ ��������� � ���������� ������, �������� ������� ����������� ������� SDK � �������������� ����� �����.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// ���� ������ ���������� ����� �������������� ������� ������������ DirectX, ������� ����� ������������ ������ ����������.
	// �������� ��������, ��� ���������� ��������� �������.
	// �� �������� �������� ����������� ��������� �������������� ������� ������ ���������� � ���
	// ��������.  ��������������, ��� ��� ���������� ������������ ������� 9.1, ���� �� ������� ����.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// �������� ������� ���������� API Direct3D 11 � ���������������� ���������.
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(
		nullptr,					// �������� nullptr ��� ������������� �������� �� ���������.
		D3D_DRIVER_TYPE_HARDWARE,	// �������� ���������� � ������� �������� ������������ ������������.
		0,							// ������ ��������� 0, ���� ������� �� ����� D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// ��������� ������ ������� � ������������� � Direct2D.
		featureLevels,				// ������ �������������� �������, ������� ����� �������������� ���� �����������.
		ARRAYSIZE(featureLevels),	// ������ ���������������� ������.
		D3D11_SDK_VERSION,			// ������ ������������� ������ D3D11_SDK_VERSION ��� ���������� ��� �������� Windows.
		&dev,					// ���������� ��������� ���������� Direct3D.
		&featureLevel,			// ���������� ������� ������� ���������� ����������.
		&context					// ���������� ���������� �������� ����������.
		);

	if (FAILED(hr))
	{
		// � ������ ���� ������������� ��������� �� ���������� WARP.

		CHECK_HRESULT(
			D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // �������� ���������� WARP ������ ����������� ����������.
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&dev,
			&featureLevel,
			&context),
			" "
			);
	}

	// ���������� ���������� �� ���������� API Direct3D 11.1 � ���������� ��������.
	CHECK_HRESULT(
		dev.As(&device), "error converting 11.0 device to 11.1"
		);

	CHECK_HRESULT(
		context.As(&devcon), "error converting 11.0 context to 11.1"
		);

	// �������� ������� ���������� Direct2D � ���������������� ���������.
	ComPtr<IDXGIDevice3> dxgiDevice;
	CHECK_HRESULT(
		device.As(&dxgiDevice), "error creating dxgi device"
		);

	CHECK_HRESULT(
		d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice),
		"error creating dxgi device"
		);

	CHECK_HRESULT(
		d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&d2dDevcon),
		"error creating 2d device context"
		);
}

void DeviceResources::AdaptToWindow(WindowDescriptor wd)
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	devcon->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	renderTargetView = nullptr;
	d2dDevcon->SetTarget(nullptr);
	d2dTargetBitmap = nullptr;
	depthStencilView = nullptr;
	devcon->Flush();

	int width = wd.size.width;
	int height = wd.size.height;

	if (width < 1) width = 1;
	if (height < 1) height = 1;

	if (swapChain != nullptr)
	{
		// ���� ������� ������� ��� ����������, �������� �� ������.
		HRESULT hr = swapChain->ResizeBuffers(
			2, // ������� ������� � ������� ������������.
			width,
			height,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		
	}
	else
	{
		// � ��������� ������ ������� ����� ����������, ��������� ��� �� �������, ��� � ��� ���������� ���������� Direct3D.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.Width = width; // � ������������ � �������� ����.
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ��� �������� ���������������� ������ ������� �������.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1; // �� ������������ ������������� �������������.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // ������������� ������� �����������, ����� ������ � �������� ��������.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // ��� ���������� ��� �������� Windows ������ ������������ ���� ������ SwapEffect.
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// ��� ������������������ �������� ������� DXGI, ������� �������������� ��� �������� �������������� ���������� Direct3D.
		ComPtr<IDXGIDevice3> dxgiDevice;
		CHECK_HRESULT(
			device.As(&dxgiDevice), 
			"error interpreting d11 device as dxgi device"
			);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		CHECK_HRESULT(
			dxgiDevice->GetAdapter(&dxgiAdapter), 
			"error getting dxgi adapter"
			);

		ComPtr<IDXGIFactory2> dxgiFactory;
		CHECK_HRESULT(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)), 
			"error getting dxgi factory from dxgi adapter"
			);

		CHECK_HRESULT(
		dxgiFactory->CreateSwapChainForHwnd(
			device.Get(),
			wd.hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain),
			" error creating swap chain from hWnd"
			);

		// �������� ����, ��� DXGI �� �������� � ������� ����� ������ ����� ������������. ��� ��������� ��������� �������� �
		// �������������, ��� ���������� ����� ��������� ���������� ������ ����� ������ ����������� �������������, ��� ������� �����������������.
		CHECK_HRESULT(
			dxgiDevice->SetMaximumFrameLatency(1), 
			"error settinf maximum frame latency"
			);
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	CHECK_HRESULT(device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&renderTargetView),
		"error creating render target view");

	// �������� ������������� ��������� ������� ��� ������������� � ���������� �����������, ���� ��� ����������.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		width,
		height,
		1, // ��� ������������� ��������� ������� �������� ������ ���� ��������.
		1, // ������������� ������ ������ MIP-����.
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	CHECK_HRESULT(
		device->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&depthStencil),
		"error creating depth stencil 2d texture"
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	CHECK_HRESULT(
		device->CreateDepthStencilView(
		depthStencil.Get(),
		&depthStencilViewDesc,
		&depthStencilView),
		"error creating depth stencil view"
		);

	// ��������� � �������� ���� ��������� ���������� ���������� ����� ����.
	viewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		width,
		height
		);

	devcon->RSSetViewports(1, &viewport);

	// �������� �������� ��������� ������� Direct2D, ���������� �
	// ������ ������� ������� �������, � ��������� ��� � �������� �������� �������� �������.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		12.0f/72.0f*96.0f,
		12.0f / 72.0f*96.0f
		);

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	CHECK_HRESULT(
		swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
		"error getting buffer from swap chain"
		);

	CHECK_HRESULT(
		d2dDevcon->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&d2dTargetBitmap),
		"error creaing bitmap from dxgi surface"
		);

	d2dDevcon->SetTarget(d2dTargetBitmap.Get());

	// ����������� ������ � �������� ������ ������������� ��� ���� ���������� ��� �������� Windows.
	d2dDevcon->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

void DeviceResources::Present()
{
	CHECK_HRESULT(swapChain->Present(1, 0),
		"Error swapChain->Present()");
}