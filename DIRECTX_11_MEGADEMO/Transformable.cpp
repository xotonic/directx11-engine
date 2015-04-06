#include "Transformable.h"

Transformable::Transformable()
{
	world = XMMatrixIdentity();
}


Transformable::Transformable(const float x /*= 0.0f*/, const float y /*= 0.0f*/, const float z /*= 0.0f*/)
{
	world = XMMatrixIdentity();
	world *= XMMatrixTranslation(x, y, z);
}

void Transformable::Translate(const float x /*= 0.0f*/, const float y /*= 0.0f*/, const float z /*= 0.0f*/)
{
	world *= XMMatrixTranslation(x, y, z);

}


void Transformable::RotateX(const float angle)
{
	world *= XMMatrixRotationX(angle);
}

void Transformable::RotateY(const float angle)
{
	world *= XMMatrixRotationY(angle);
}

void Transformable::RotateZ(const float angle)
{
	world *= XMMatrixRotationZ(angle);
}

void Transformable::LookAt(const XMFLOAT3 focus)
{
	world *= XMMatrixLookAtLH(
		XMVectorSet(world._14, world._24, world._34, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

}

void Transformable::Scale(float value)
{
	world *= XMMatrixScaling(value, value, value);
}
