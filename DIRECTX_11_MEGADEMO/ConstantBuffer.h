#pragma once
#include "common.h"

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device);

	void update();
	void bind(unsigned int slot);
	//~ConstantBuffer();
	T data;
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* cbuf;
};

template <typename T>
ConstantBuffer<T>::ConstantBuffer(ID3D11Device* device) : dev(device)
{
	dev->GetImmediateContext(&deviceContext);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(T);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	CHECK_HRESULT(dev->CreateBuffer(&bd, NULL, &cbuf), "Error creating constant buffer");
}



template <typename T>
void ConstantBuffer<T>::bind(unsigned int slot)
{

	deviceContext->VSSetConstantBuffers(0, 1, &cbuf);
}

template <typename T>
void ConstantBuffer<T>::update()
{
	deviceContext->UpdateSubresource(cbuf, 0, 0, &data, 0, 0);

}