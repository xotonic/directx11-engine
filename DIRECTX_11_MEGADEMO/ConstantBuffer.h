#pragma once
#include "common.h"

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device);

	void update();
	void bind(unsigned int slot);
	~ConstantBuffer();
	T data;
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* cbuf;
};


