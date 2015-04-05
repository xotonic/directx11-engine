#pragma once

#include "common.h"
#include <vector>
using namespace std;

class IndexBuffer
{
public:
	IndexBuffer(ID3D11Device* dev, vector<unsigned int> &data);
	~IndexBuffer();
	int getSize();
	int getCount();
	void bind();
protected:

	vector<unsigned int> i_buf;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer*         indexBuffer = NULL;
};

