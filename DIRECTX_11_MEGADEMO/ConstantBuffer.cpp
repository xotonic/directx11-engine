//#include "ConstantBuffer.h"
//
//
//template <typename T>
//ConstantBuffer<T>::ConstantBuffer(ID3D11Device* device) : dev(device), data(0)
//{
//	dev->GetImmediateContext(&deviceContext);
//
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(T);
//	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bd.CPUAccessFlags = 0;
//
//	CHECK_HRESULT(dev->CreateBuffer(&bd, NULL, &cbuf), "Error creating constant buffer");
//}
//
//
//
//template <typename T>
//void ConstantBuffer<T>::bind(unsigned int slot)
//{
//	
//	deviceContext->VSSetConstantBuffers(0, 1, &cbuf);
//}
//
//template <typename T>
//void ConstantBuffer<T>::update()
//{
//	deviceContext->UpdateSubresource(cbuf, 0, 0, &data, 0, 0);
//
//}
//
