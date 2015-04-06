#include "Texture.h"

Texture::Texture(ID3D11Device* dev, LPCWSTR filename) : DeviceDependent(dev)
{
	//dev->GetImmediateContext(&deviceContext);

	CHECK_HRESULT(CreateDDSTextureFromFile(device, filename, nullptr, &textureRV),
		"ERROR creating dds texture from file");
}

void Texture::bind(unsigned int startSlot)
{
	deviceContext->PSSetShaderResources(startSlot, 1, &textureRV);
	deviceContext->PSSetSamplers(0, 0, 0);
}