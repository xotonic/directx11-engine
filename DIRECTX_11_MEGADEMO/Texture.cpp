#include "Texture.h"

Texture::Texture(ID3D11Device* dev, std::string filename) : DeviceDependent(dev)
{

	Debug::if_failed(CreateDDSTextureFromFile(device, stringToWstring(filename).c_str(), nullptr, &textureRV),
		"ERROR creating dds texture from file");
}

void Texture::bind(unsigned int startSlot)
{
	deviceContext->PSSetShaderResources(startSlot, 1, &textureRV);
	deviceContext->PSSetSamplers(0, 0, 0);
}