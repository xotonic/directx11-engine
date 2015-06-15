#pragma once
#include "stdafx.h"

class Transformable
{
public:
	Transformable();
	Transformable& operator=(const Transformable& Transformable);
	Transformable(const Transformable& Transformable);
	void  Move(XMFLOAT3 direction);
	void  Rotate(XMFLOAT3 axis, float degrees);
	void  Position(XMFLOAT3& new_position);
	void  Scale(float value);
	const XMFLOAT3& Position() const { return mPosition; }
	const XMMATRIX World() { return XMMatrixTranspose(mWorld); }
	void initWorldMatrix();

private:

	XMFLOAT3 dir;

	XMFLOAT3 mPosition;
	XMFLOAT3 mTarget;
	XMFLOAT3 mUp;

	XMMATRIX  mWorld;
};
