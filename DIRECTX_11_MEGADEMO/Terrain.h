#pragma once

#include <string>

#include "stdafx.h"
#include "VertexBuffer.h"
#include "DeviceDependent.h"

static const XMFLOAT3 operator-(XMFLOAT3 x, XMFLOAT3 y)
	{
	return XMFLOAT3{x.x-y.x, x.y - y.y, x.z - y.z};
	}

class Terrain : DeviceDependent
{
public:
	Terrain(ID3D11Device *dev, std::string file);
	~Terrain();

	vector<TexNormVertex> v_buf;

	int getSize();
	int getCount();
	void draw();
	void bind();

	ID3D11Buffer*         vertexBuffer = nullptr;
private:
	bool ReadFromFile(std::string name);

};
