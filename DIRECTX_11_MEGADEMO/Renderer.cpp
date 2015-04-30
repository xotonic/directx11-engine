#include "Renderer.h"

Renderer::Renderer(WindowDescriptor wd) : angle(0)
{
	createDevice(wd);

	world = XMMatrixIdentity();
	view = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, wd.size.width / wd.size.height, 0.01f, 1000.0f);

	resMgr = new ResourceManager(device);

	terrain = new Terrain(device, "out.terrain");

	resMgr->loadShader("simple", "shader.vsh", "shader.psh");
	resMgr->loadMesh("cube", "mon.mesh");
	resMgr->loadTexture("diffuse", "grass.dds");
	resMgr->loadTexture("normal", "grass_normal.dds");

	resMgr->mesh("cube")->bind();
	resMgr->shader("simple")->bind();

	camera.Translate(0.0f, 1.0, -5.0);

	matrices = new ConstantBuffer < MatrixBuffer >(device);
	light = new ConstantBuffer<DirLightBuffer>(device);

	light->data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->data.dir = XMFLOAT4(-0.577f, 0.0f, -0.5f, 1.0f);

	light->update();
	light->bind_PS(0);
	/*light->update();
	light->bind_PS(1);*/

	tr.Translate(4.0f, 4.0, 0.0);
	//tex = new Texture(device, "texture.dds");
	//normal = new Texture(device, "normal.dds");

	/*D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &sampler);

	CHECK_HRESULT(CreateDDSTextureFromFile(device, L"texture.dds", nullptr, &textureRV),
	"ERROR creating dds texture from file");*/

	//deviceContext->VSSetShader(shader->vertex(), 0, 0);
	//deviceContext->PSSetShader(shader->pixel(), 0, 0);

	/*D3D11_RASTERIZER_DESC noCulling;
	ID3D11RasterizerState* noCullingState;
	ZeroMemory(&noCulling, sizeof(D3D11_RASTERIZER_DESC));
	noCulling.FillMode = D3D11_FILL_WIREFRAME;
	noCulling.CullMode = D3D11_CULL_NONE;
	CHECK_HRESULT(
	device->CreateRasterizerState(&noCulling, &noCullingState),
	"Cannot create rasterizer state");
	deviceContext->RSSetState(noCullingState);*/
}

Renderer::~Renderer()
{
	//delete shader;
	//delete buf;
	delete matrices;
	delete light;
	delete terrain;
	//delete tex;
	//delete normal;
	delete resMgr;
	//delete ibuf;

	if (deviceContext)    deviceContext->ClearState();
	if (depthTexture)     depthTexture->Release();
	if (depthStencilView) depthStencilView->Release();
	if (renderTargetView) renderTargetView->Release();
	if (swapChain)        swapChain->Release();
	if (deviceContext)    deviceContext->Release();
	if (device)           device->Release();
}

void Renderer::createDevice(WindowDescriptor wd)
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
	UINT flags = NULL;
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
		DEBUG("CREATE DEVICE ERROR\n");
		return;
	}

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		DEBUG("ERROR GETTING BACK BUFFER\n");
		return;
	}

	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		DEBUG("ERROR CREATING RENDER TARGET VIEW\n");
		return;
	}

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
}

void Renderer::Render()
{
	float clearColor[4] = { 0.30f, 0.30f, 0.30f, 1.0f };

	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	static float t = 0.0f;
	t += 0.0005;

	resMgr->mesh("cube")->bind();

	light->update();
	light->bind_PS(0);

	matrices->data.world = transform.get();//XMMatrixTranspose(world);
	matrices->data.view = camera.get();// XMMatrixTranspose(view);
	matrices->data.projection = XMMatrixTranspose(projection);

	matrices->update();
	matrices->bind_VS(0);

	resMgr->texture("diffuse")->bind(0);
	resMgr->texture("normal")->bind(1);

	//deviceContext->PSSetShaderResources(0, 1, &textureRV);
	//deviceContext->PSSetSamplers(0, 0, 0);
	resMgr->mesh("cube")->draw();

	matrices->data.world = tr.get();//XMMatrixTranspose(world);
	matrices->data.view = camera.get();// XMMatrixTranspose(view);
	matrices->data.projection = XMMatrixTranspose(projection);

	matrices->update();
	matrices->bind_VS(0);
	light->bind_PS(0);


	//light->update();

	//light->update();
	//light->bind(1);
	resMgr->mesh("cube")->draw();

	terrain->bind();

	matrices->data.world = transform.get();
	matrices->update();
	matrices->bind_VS(0);

	terrain->draw();
//	deviceContext->Draw(resMgr->mesh("cube")->getCount(), 0);

	swapChain->Present(0, 0);
}