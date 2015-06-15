#include "Camera.h"

Camera::Camera(void)
{
	mPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mUp = to(to(mPosition) + to(XMFLOAT3(0, 1, 0)));
	this->initViewMatrix();

	mAngle = 0.0f;
	mClientWidth = 0.0f;
	mClientHeight = 0.0f;
	mNearest = 0.0f;
	mFarthest = 0.0f;

	XMStoreFloat4x4(&mView, XMMatrixIdentity());
	XMStoreFloat4x4(&mProj, XMMatrixIdentity());
	XMStoreFloat4x4(&mOrtho, XMMatrixIdentity());
}

Camera::Camera(const Camera& camera)
{
	*this = camera;
}

Camera& Camera::operator=(const Camera& camera)
{
	mPosition = camera.mPosition;
	mTarget = camera.mTarget;
	mUp = camera.mUp;

	mAngle = camera.mAngle;
	mClientWidth = camera.mClientWidth;
	mClientHeight = camera.mClientHeight;
	mNearest = camera.mNearest;
	mFarthest = camera.mFarthest;

	mView = camera.mView;
	mProj = camera.mProj;
	mOrtho = camera.mOrtho;
	return *this;
}

void Camera::initViewMatrix()
{
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&mTarget),
		XMLoadFloat3(&this->Up())));
}

void Camera::InitProjMatrix(const float angle, const float client_width, const float client_height,
	const float near_plane, const float far_plane)
{
	mAngle = angle;
	mClientWidth = client_width;
	mClientHeight = client_height;
	mNearest = near_plane;
	mFarthest = far_plane;
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(angle, 1.0f,
		near_plane, far_plane));
}

void Camera::Move(XMFLOAT3 direction)
{
	mPosition = to(XMVector3Transform(to(mPosition),
		XMMatrixTranslation(direction.x, direction.y, direction.z)));
	mTarget = to(XMVector3Transform(to(mTarget),
		XMMatrixTranslation(direction.x, direction.y, direction.z)));
	mUp = to(XMVector3Transform(to(mUp),
		XMMatrixTranslation(direction.x, direction.y, direction.z)));

	this->initViewMatrix();
}

void Camera::Rotate(XMFLOAT3 axis, float degrees)
{
	if (XMVector3Equal(to(axis), XMVectorZero()) ||
		degrees == 0.0f)
		return;

	// rotate vectors
	XMFLOAT3 look_at_target = to(to(mTarget) - to(mPosition));
	XMFLOAT3 look_at_up = to(to(mUp) - to(mPosition));
	look_at_target = to(XMVector3Transform(to(look_at_target),
		XMMatrixRotationAxis(to(axis), XMConvertToRadians(degrees))));
	look_at_up = to(XMVector3Transform(to(look_at_up),
		XMMatrixRotationAxis(to(axis), XMConvertToRadians(degrees))));

	// restore vectors's end points mTarget and mUp from new rotated vectors
	mTarget = to(to(mPosition) + to(look_at_target));
	mUp = to(to(mPosition) + to(look_at_up));

	this->initViewMatrix();

	XMVECTOR w = XMVectorZero();
}

void Camera::Target(XMFLOAT3 new_target)
{
	if (XMVector3Equal(to(new_target), to(mPosition)) ||
		XMVector3Equal(to(new_target), to(mTarget)))
		return;

	XMFLOAT3 old_look_at_target = to(to(mTarget) - to(mPosition));
	XMFLOAT3 new_look_at_target = to(to(new_target) - to(mPosition));
	float angle = XMConvertToDegrees(XMVectorGetX(
		XMVector3AngleBetweenNormals(XMVector3Normalize(to(old_look_at_target)),
		XMVector3Normalize(to(new_look_at_target)))));
	if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
	{
		XMVECTOR axis = XMVector3Cross(to(old_look_at_target), to(new_look_at_target));
		Rotate(to(axis), angle);
	}
	mTarget = new_target;
	this->initViewMatrix();
}

// Set camera position
void Camera::Position(XMFLOAT3& new_position)
{
	XMFLOAT3 move_vector = to(to(new_position) - to(mPosition));
	XMFLOAT3 target = mTarget;
	this->Move(move_vector);
	this->Target(target);
}

void Camera::Angle(float angle)
{
	mAngle = angle;
	InitProjMatrix(mAngle, mClientWidth, mClientHeight, mNearest, mFarthest);
}

void Camera::NearestPlane(float nearest)
{
	mNearest = nearest;
	OnResize(mClientWidth, mClientHeight);
}

void Camera::FarthestPlane(float farthest)
{
	mFarthest = farthest;
	OnResize(mClientWidth, mClientHeight);
}

void Camera::InitOrthoMatrix(const float clientWidth, const float clientHeight,
	const float nearZ, const float fartherZ)
{
	XMStoreFloat4x4(&mOrtho, XMMatrixOrthographicLH(clientWidth, clientHeight, 0.0f, fartherZ));
}

void Camera::OnResize(uint32_t new_width, uint32_t new_height)
{
	mClientWidth = new_width;
	mClientHeight = new_height;
	InitProjMatrix(mAngle, static_cast<float>(new_width), static_cast<float>(new_height), mNearest, mFarthest);
	InitOrthoMatrix(static_cast<float>(new_width), static_cast<float>(new_height), 0.0f, mFarthest);
}