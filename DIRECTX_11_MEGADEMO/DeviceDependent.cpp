#include "DeviceDependent.h"

DeviceDependent::DeviceDependent(ID3D11Device * dev) : device(dev)
{
	dev->GetImmediateContext(&deviceContext);
}