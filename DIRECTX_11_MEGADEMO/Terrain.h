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
	return XMFLOAT3{x.x-y.x, x.y - y.y, x.z - y.z};
	}

struct Quad { XMVECTOR top_left, top_right, bot_right, bot_left; 
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

	XMVECTOR findIntersection(const VectorPair& ray, Quad& quad, Console& c)
	{

		//XMVECTOR point = XMVectorZero();

		//float dx = XMVectorGetX(ray.first) - XMVectorGetX(ray.second);
		//float dz = XMVectorGetZ(ray.first) - XMVectorGetZ(ray.second);

		//int l = max(std::round(dx), std::round(dz)) + 1;

		//float x = XMVectorGetX(ray.first), z = XMVectorGetZ(ray.first);

		//for (auto bb : boxes)
		//{
		//	if (bb.Intersects(ray.first, XMVector3Normalize(ray.second), *new float))
		//	{
		//		Quad q = getQuad(bb.Center.x, bb.Center.z);
		//		quad = q;
		//		return to(bb.Center);
		//	}
		//}

		//
		//for (int x = 0; x < 253 ; x++)
		//	for (int y = 0; y < 253; y++)
		//{
		//	
		//	Quad q = getQuad(x, y);
		//	quad = q;
		//	float dist;
		//	if (TriangleTests::Intersects(ray.first, XMVector3Normalize(ray.second), q.top_left, q.top_right, q.bot_right, dist))
		//	{
		//		//return  ray .first + XMVector3Normalize(ray.second)*dist;
		//		return XMPlaneIntersectLine(XMPlaneFromPoints(q.top_left, q.top_right, q.bot_right), ray.first, ray.second);
		//		
		//	}
		//	
		//	if (TriangleTests::Intersects(ray.first, XMVector3Normalize(ray.second), q.top_left, q.bot_right, q.bot_left, dist))
		//	{
		//		//return ray.first + XMVector3Normalize(ray.second)*dist;
		//		return	XMPlaneIntersectLine(XMPlaneFromPoints(q.top_left, q.bot_right, q.bot_left), ray.first, ray.second);
		//		
		//	}
		//}

		float origin[3] = { XMVectorGetX(ray.first), XMVectorGetY(ray.first), XMVectorGetZ(ray.first) };
		float dir[3] = { XMVectorGetX(ray.second), XMVectorGetY(ray.second), XMVectorGetZ(ray.second) };
		if (model->rayCollision(origin, dir))
		{
			float point[3] = { 0 };
			model->getCollisionPoint(point);
			XMVECTOR p = XMVectorSet(point[0], point[1], point[2], 0.0f) - 0.04*ray.first;
			//p = p - (p - ray.first)*0.05;
			return p;
		}
		else
		{
			return XMVectorZero();
		}
	}
	Quad getQuad(int x, int y)
	{
		std::swap(x, y);
		int i = 6 * (x + (wight-1)*y);
		if (i + 5 > v_buf.size()) {
			ostringstream o;
			o << "index out\n x = " << x << "\n y = " << y;
			MESSAGE(o.str());
		}
		return { to(v_buf[i].pos), to(v_buf[i + 1].pos), to(v_buf[i + 2].pos), to(v_buf[i + 5].pos) };
	}
	Quad getQuad(float x, float y) { return getQuad((int)std::floor(x), (int)std::floor(y)); };

	ID3D11Buffer*         vertexBuffer = nullptr;
private:
	bool ReadFromFile(std::string name);

	int height, wight;
	CollisionModel3D *model;
	std::vector<BoundingBox> boxes;
};
