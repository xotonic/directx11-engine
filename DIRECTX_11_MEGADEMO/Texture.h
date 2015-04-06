#pragma once
#include "common.h"
#include "DeviceDependent.h"
#include "DDSTextureLoader\DDSTextureLoader.h"
class Texture : private DeviceDependent
{
public:
	Texture(ID3D11Device* dev, LPCWSTR filename);
	void bind(unsigned int startSlot);

private:
	ID3D11ShaderResourceView *textureRV = nullptr;
};
