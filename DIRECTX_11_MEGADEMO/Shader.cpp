#include "Shader.h"

Shader::Shader(ID3D11Device* dev, std::string vs_name, std::string ps_name, bool _color, bool _normal, bool _uv)
	: DeviceDependent(dev), colorComponent(_color), normalComponent(_normal), UVComponent(_uv)

{
	//dev->GetImmediateContext(&deviceContext);

	ID3D10Blob* vsBlob = NULL;
	ID3D10Blob* psBlob = NULL;
	compileFromFile(vs_name, VS_entryPoint, VS_model, &vsBlob);

	Debug::if_failed(
		dev->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		0,
		&vertexShader),
		"error creating vertex shader");

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	if (_color)
		desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	if (_normal)
		desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	if (_uv)
		desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	Debug::if_failed(
		dev->CreateInputLayout(desc.data(), desc.size(), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &inputLayout), "error creating input layout");

	vsBlob->Release();

	compileFromFile(ps_name, PS_entryPoint, PS_model, &psBlob);
	Debug::if_failed(
		dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader),
		"error creating pixel shader");

	psBlob->Release();
}

Shader::~Shader()
{
	if (inputLayout) inputLayout->Release();

	if (vertexShader) vertexShader->Release();

	if (pixelShader) pixelShader->Release();
}

void Shader::compileFromFile(std::string fileName, const LPCSTR ep, const LPCSTR model, ID3D10Blob** blob)
{
	HRESULT hr = S_OK;
	ID3DBlob* errorBlob = NULL;

	hr = D3DCompileFromFile(stringToWstring(fileName).c_str(), 0, 0, ep, model, 0, 0, blob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob != 0)
		{
			Debug::message((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			return;
		}
		else
		{
			Debug::message("error compiling shader from file");
			return;
		}
	}
}

void Shader::bind()
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);
}