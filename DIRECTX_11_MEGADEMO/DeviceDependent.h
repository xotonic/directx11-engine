#pragma once
#include "common.h"
class DeviceDependent
{
protected:
	DeviceDependent(ID3D11Device * dev);
	
	ID3D11Device* device= nullptr;
	ID3D11DeviceContext *deviceContext = nullptr;
};

