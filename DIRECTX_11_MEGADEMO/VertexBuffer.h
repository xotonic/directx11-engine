#pragma once
#include <vector>
#include <fstream>
#include "common.h"
#include "DeviceDependent.h"
using namespace std;
class VertexBuffer : private DeviceDependent
{
public:
	VertexBuffer(ID3D11Device* dev, vector<TexNormVertex> &data);
	VertexBuffer(ID3D11Device* dev, string filename);
	~VertexBuffer();
	int getSize();
	int getCount();
	void bind();
protected:

	bool ReadFromOBJ(string filename);
	vector<TexNormVertex> v_buf;
	//ID3D11Device* device;
	//ID3D11DeviceContext* deviceContext;
	ID3D11Buffer*         vertexBuffer = NULL;
};
