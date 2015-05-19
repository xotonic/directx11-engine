#pragma once
#include "stdafx.h"
#include "DeviceDependent.h"

template <typename T>
class ConstantBuffer : private DeviceDependent
{
public:
	ConstantBuffer(ID3D11Device* dev);

	void update();
	void bind_VS(unsigned int slot);
	void bind_PS(unsigned int slot);
	//~ConstantBuffer();
	T data;
private:
	//ID3D11Device* dev;
	//ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* cbuf;
};

template <typename T>
void ConstantBuffer<T>::bind_PS(unsigned int slot)
{
	deviceContext->PSSetConstantBuffers(slot, 1, &cbuf);
}

template <typename T>
void ConstantBuffer<T>::bind_VS(unsigned int slot)
{
	deviceContext->VSSetConstantBuffers(slot, 1, &cbuf);
}
template <typename T>
ConstantBuffer<T>::ConstantBuffer(ID3D11Device* dev) : DeviceDependent(dev)
{
	//device->GetImmediateContext(&deviceContext);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(T);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	CHECK_HRESULT(device->CreateBuffer(&bd, NULL, &cbuf), "Error creating constant buffer");
}


template <typename T>
void ConstantBuffer<T>::update()
{

	deviceContext->UpdateSubresource(cbuf, 0, 0, &data, 0, 0);
}