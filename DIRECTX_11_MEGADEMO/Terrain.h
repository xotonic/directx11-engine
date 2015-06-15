#pragma once

#include <string>
#include <array>
#include <cmath>
#include <DirectXCollision.h>
#include "stdafx.h"
#include "VertexBuffer.h"
#include "DeviceDependent.h"
#include "Console.h"
#include "ColDet/coldet.h"
static const XMFLOAT3 operator-(XMFLOAT3 x, XMFLOAT3 y)
{
	return XMFLOAT3{ x.x - y.x, x.y - y.y, x.z - y.z };
}

struct Quad {
	XMVECTOR top_left, top_right, bot_right, bot_left;
	float maxHeigth()
	{
		return max(max(XMVectorGetY(top_left), XMVectorGetY(top_right)), max(XMVectorGetY(bot_left), XMVectorGetY(bot_right)));
	}
};

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

	Quad getQuad(int x, int y)
	{
		std::swap(x, y);
		int i = 6 * (x + (wight - 1)*y);
		if (i + 5 > v_buf.size()) {
			ostringstream o;
			o << "index out\n x = " << x << "\n y = " << y;
			MESSAGE(o.str());
		}
		return{ to(v_buf[i].pos), to(v_buf[i + 1].pos), to(v_buf[i + 2].pos), to(v_buf[i + 5].pos) };
	}
	Quad getQuad(float x, float y) { return getQuad((int)std::floor(x), (int)std::floor(y)); };

	ID3D11Buffer*         vertexBuffer = nullptr;
	int height, wight;
private:
	bool ReadFromFile(std::string name);
};
