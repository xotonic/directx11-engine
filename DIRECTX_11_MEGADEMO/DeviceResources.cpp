#include "DeviceResources.h"


DeviceResources::DeviceResources()
{
	CreateFactories();
	CreateDeviceResources();
}


void DeviceResources::CreateFactories()
{
	// Инициализация ресурсов Direct2D.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// Если проект не является отладочной сборкой, включите отладку Direct2D с помощью уровней SDK.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// Инициализация фабрики Direct2D.
	CHECK_HRESULT(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		&d2dFactory),
		"error creating Direct2d factory"
		);

	// Инициализация фабрики DirectWrite.
	CHECK_HRESULT(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&dwriteFactory),
		"error creating DirectWrite factory"
		);

	// Инициализация фабрики компонента обработки изображений Windows (WIC).
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
	// Этот флаг добавляет поддержку поверхностей с другим порядком цветовых каналов
	// по сравнению с API по умолчанию. Он является обязательным для совместимости с Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		// Если проект находится в отладочной сборке, включите отладку посредством уровней SDK с использованием этого флага.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// Этот массив определяет набор функциональных уровней оборудования DirectX, которые будет поддерживать данное приложение.
	// Обратите внимание, что необходимо сохранить порядок.
	// Не забудьте объявить минимальный требуемый функциональный уровень вашего приложения в его
	// описании.  Предполагается, что все приложения поддерживают уровень 9.1, если не указано иное.
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

	// Создание объекта устройства API Direct3D 11 и соответствующего контекста.
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(
		nullptr,					// Указание nullptr для использования адаптера по умолчанию.
		D3D_DRIVER_TYPE_HARDWARE,	// Создание устройства с помощью драйвера графического оборудования.
		0,							// Должно равняться 0, если драйвер не равен D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// Установка флагов отладки и совместимости с Direct2D.
		featureLevels,				// Список функциональных уровней, которые могут поддерживаться этим приложением.
		ARRAYSIZE(featureLevels),	// Размер вышеприведенного списка.
		D3D11_SDK_VERSION,			// Всегда устанавливать равным D3D11_SDK_VERSION для приложений для Магазина Windows.
		&dev,					// Возвращает созданное устройство Direct3D.
		&featureLevel,			// Возвращает уровень функций созданного устройства.
		&context					// Возвращает мгновенный контекст устройства.
		);

	if (FAILED(hr))
	{
		// В случае сбоя инициализации вернитесь на устройство WARP.

		CHECK_HRESULT(
			D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Создание устройства WARP вместо аппаратного устройства.
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

	// Сохранение указателей на устройство API Direct3D 11.1 и мгновенный контекст.
	CHECK_HRESULT(
		dev.As(&device), "error converting 11.0 device to 11.1"
		);

	CHECK_HRESULT(
		context.As(&devcon), "error converting 11.0 context to 11.1"
		);

	// Создание объекта устройства Direct2D и соответствующего контекста.
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
		// Если цепочка буферов уже существует, изменяем ее размер.
		HRESULT hr = swapChain->ResizeBuffers(
			2, // Цепочка буферов с двойной буферизацией.
			width,
			height,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		
	}
	else
	{
		// В противном случае создаем новое устройство, используя тот же адаптер, что и для имеющегося устройства Direct3D.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.Width = width; // В соответствии с размером окна.
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Это наиболее распространенный формат цепочки буферов.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1; // Не использовать множественную дискретизацию.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // Использование двойной буферизации, чтобы свести к минимуму задержку.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // Все приложения для Магазина Windows должны использовать этот эффект SwapEffect.
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// Эта последовательность получает фабрику DXGI, которая использовалась для создания вышеуказанного устройства Direct3D.
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

		// Проверка того, что DXGI не помещает в очередь более одного кадра одновременно. Это позволяет уменьшить задержку и
		// гарантировать, что приложение будет выполнять прорисовку только после каждой виртуальной синхронизации, что снижает энергопотребление.
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

	// Создание представления трафарета глубины для использования с трехмерной прорисовкой, если это необходимо.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		width,
		height,
		1, // Это представление трафарета глубины содержит только одну текстуру.
		1, // Использование одного уровня MIP-карт.
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

	// Установка в качестве окна просмотра трехмерной прорисовки всего окна.
	viewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		width,
		height
		);

	devcon->RSSetViewports(1, &viewport);

	// Создание целевого точечного рисунка Direct2D, связанного с
	// задним буфером цепочки буферов, и установка его в качестве текущего целевого объекта.
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

	// Сглаживание текста в оттенках серого рекомендуется для всех приложений для Магазина Windows.
	d2dDevcon->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

void DeviceResources::Present()
{
	CHECK_HRESULT(swapChain->Present(1, 0),
		"Error swapChain->Present()");
}
