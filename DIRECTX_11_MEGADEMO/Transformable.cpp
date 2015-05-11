#include "Transformable.h"

Transformable::Transformable()
{
	world = XMMatrixIdentity();
	pos = XMVectorSet(0, 0, 0, 1.0f);
	dir = base_dir;
	rot = XMVectorSet(0, 0, 0, 1);
	scale = XMVectorSet(1, 1, 1, 1);
}


void Transformable::Translate(XMFLOAT3 vec)
{
	
	world *= XMMatrixTranslationFromVector(to(vec));
	pos += XMVector3Rotate(to(vec), dir2angles(dir));
}


void Transformable::RotateX(const float angle)
{
	world *= XMMatrixRotationX(angle);
	//rot += XMVectorSet(angle, 0, 0, 1);
	dir += XMVector3Rotate(dir, XMVectorSet(angle, 0, 0, 1));
}

void Transformable::RotateY(const float angle)
{
	world *= XMMatrixRotationY(angle);
	//rot += XMVectorSet(0, angle, 0, 1);
	dir += XMVector3Rotate(dir, XMVectorSet(0, angle, 0, 1));
}

void Transformable::RotateZ(const float angle)
{
	world *= XMMatrixRotationZ(angle);
	//rot += XMVectorSet(0, 0, angle, 1);
	dir += XMVector3Rotate(dir, XMVectorSet(0, 0, angle, 1));
}

void Transformable::LookAt(const XMFLOAT3 focus)
{
	
	XMVECTOR v = XMLoadFloat3(&focus);
	dir = XMVector3Normalize(v);
	
	world *= XMMatrixLookAtLH(
		pos,
		v,
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Transformable::Scale(float value)
{
	world *= XMMatrixScaling(value, value, value);
}

XMFLOAT3 Transformable::getPos() 
{ 
	float x = world.r[0].vector4_f32[3];
	float y = world.r[1].vector4_f32[3];
	float z = world.r[2].vector4_f32[3];
	XMFLOAT3 p(x,y,z);
	//XMStoreFloat3(&p, pos);
	return p;
}

XMVECTOR& Transformable::dir2angles(XMVECTOR& dir)
{
	XMVECTOR q;
	q = XMVector3Cross(base_dir, dir);

	float a = XMVector3LengthSq(base_dir).vector4_f32[0];
	float b = XMVector3LengthSq(dir).vector4_f32[0];
	float dot = XMVector3Dot(base_dir, dir).vector4_f32[0];
	q.vector4_f32[3] = sqrtf(a * b) + dot;

	return XMQuaternionNormalize(q);
}
