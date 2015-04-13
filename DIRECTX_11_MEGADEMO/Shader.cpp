#include "Shader.h"

Shader::Shader(ID3D11Device* dev, std::string vs_name, std::string ps_name)
	: DeviceDependent(dev)
{

	//dev->GetImmediateContext(&deviceContext);

	ID3D10Blob* vsBlob = NULL;
	ID3D10Blob* psBlob = NULL;
	compileFromFile(vs_name, VS_entryPoint, VS_model, &vsBlob);

	dev->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		0,
		&vertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	dev->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &inputLayout);

	vsBlob->Release();

	compileFromFile(ps_name, PS_entryPoint, PS_model, &psBlob);
	dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader);

	psBlob->Release();
	deviceContext->IASetInputLayout(inputLayout);
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

	//MESSAGE(fileName);

	//LPCWSTR s = (LPCWSTR)stringToLPCWSTR(fileName) + L'\0';
	//std::wstring s(L"simple.fx");
	hr = D3DCompileFromFile(stringToWstring(fileName).c_str(), 0, 0, ep, model, 0, 0, blob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob != 0)
		{
			DEBUG((char*)errorBlob->GetBufferPointer());
			MESSAGE((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			return;
		}
		else
		{
			MESSAGE("error compiling shader from file");
			return;
			//throw "can't compile shader from ";
		}
	}
}

void Shader::bind()
{
	deviceContext->VSSetShader(vertexShader, 0, 0);
	//deviceContext->VSSetConstantBuffers(0, 1, constBuf);
	deviceContext->PSSetShader(pixelShader, 0, 0);
}

void Shader::setConstantBuffers(ID3D11Buffer** bufs)
{

}
