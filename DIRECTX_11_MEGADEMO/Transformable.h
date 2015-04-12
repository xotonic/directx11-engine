#pragma once
#include "common.h"

class Transformable
{
public:
	Transformable();
	Transformable(const float x, const float y = 0.0f, const float z = 0.0f);

	static XMFLOAT3 up() {	return XMFLOAT3(0.0f, 1.0f, 0.0f); }
	static XMFLOAT3 down() { return XMFLOAT3(0.0f, -1.0f, 0.0f); }
	//static XMFLOAT3 forward() { return XMFLOAT3(0.0f, 0.0f, 1.0f); }

	void Translate(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);

	//void Rotate(const XMFLOAT3 vec);
	void RotateX(const float angle);
	void RotateY(const float angle);
	void RotateZ(const float angle);
	void LookAt(const XMFLOAT3 focus);

	void Scale(float value);

	XMMATRIX get() { return XMMatrixTranspose(world); }
	XMFLOAT3 getPos();// { return XMFLOAT3(world._14, world._24, world._34); }
	XMMATRIX world;
};

