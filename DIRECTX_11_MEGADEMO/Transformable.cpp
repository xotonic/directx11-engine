#include "Transformable.h"

Transformable::Transformable(void)
{
	mPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mUp = to(to(mPosition) + to(XMFLOAT3(0, 1, 0)));
	this->initWorldMatrix();

	mWorld = XMMatrixIdentity();

}

Transformable::Transformable(const Transformable& Transformable)
{
	*this = Transformable;
}

Transformable& Transformable::operator=(const Transformable& Transformable)
{
	mPosition = Transformable.mPosition;
	mTarget = Transformable.mTarget;
	mUp = Transformable.mUp;

	mWorld = Transformable.mWorld;
	
	return *this;
}

void Transformable::initWorldMatrix()
{
	XMVECTOR pos = to(mPosition);
	XMVECTOR eye = to(mTarget);
	XMVECTOR up = to(mUp);

	//XMVectorSetW(pos, 1.0f);
	//XMVectorSetW(eye, 1.0f);
	//XMVectorSetW(up, 1.0f);

	//eye = XMVector3Normalize(eye);
	//up = XMVector3Normalize(up);

	if (XMVector3Equal(up, XMVectorZero()))
	{
	up = XMVectorSet(0.0f, 1.0f, 0.0f ,0.0f);
	}
	mWorld = XMMatrixLookAtLH(pos, eye, up);
}


void Transformable::Move(XMFLOAT3 direction)
{
	//XMVECTOR v = to(direction);
	//mPosition = to(to(mPosition) + v);
	//mTarget = to(to(mTarget) + v);
	//mUp = to(to(mUp) + v);
	XMMATRIX m =XMMatrixTranslation(direction.x, direction.y, direction.z);
	mPosition = to(XMVector3TransformCoord(to(mPosition),m));
	mWorld *= m;
}

void Transformable::Rotate(XMFLOAT3 axis, float degrees)
{
	if (XMVector3Equal(to(axis), XMVectorZero()) ||
		degrees == 0.0f)
		return;

	// rotate vectors
	XMFLOAT3 look_at_target = to(to(mTarget) - to(mPosition));
	XMFLOAT3 look_at_up = to(to(mUp) - to(mPosition));
	look_at_target = to(XMVector3TransformCoord(to(look_at_target),
		XMMatrixRotationAxis(to(axis), XMConvertToRadians(degrees))));
	look_at_up = to(XMVector3TransformCoord(to(look_at_up),
		XMMatrixRotationAxis(to(axis), XMConvertToRadians(degrees))));

	// restore vectors's end points mTarget and mUp from new rotated vectors
	mTarget = to(to(mPosition) + to(look_at_target));
	mUp = to(to(mPosition) + to(look_at_up));

	this->initWorldMatrix();
}

// Set Transformable position
void Transformable::Position(XMFLOAT3& new_position)
{
	mPosition = new_position;
	XMMATRIX m = XMMatrixTranslationFromVector(to(new_position));
	mWorld = m;
	//initWorldMatrix();
	//this->Move(move_vector);
	//this->Target(target);
}


