#pragma once

#include <string>

#include "common.h"
#include "VertexBuffer.h"
#include "DeviceDependent.h"

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
