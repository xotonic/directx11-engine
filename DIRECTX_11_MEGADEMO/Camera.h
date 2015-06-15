#pragma once

#include "stdafx.h"


class Camera 
{
public:
	// Constructs default camera looking at 0,0,0
	// placed at 0,0,-1 with up vector 0,1,0 (note that mUp is NOT a vector - it's vector's end)
	Camera(void);
	// Create camera, based on another one
	Camera(const Camera& camera);
	// Copy all camera's parameters
	Camera& operator=(const Camera& camera);
	~Camera(void) {}

private:
	// Initialize camera's View matrix from mPosition, mTarget and mUp coordinates
	void initViewMatrix();

public:
	void InitProjMatrix(const float angle, const float client_width, const float client_height,
		const float nearest, const float farthest);
	void InitOrthoMatrix(const float client_width, const float client_height,
		const float near_plane, const float far_plane);

	void OnResize(uint32_t new_width, uint32_t new_height);

	
	void  Move(XMFLOAT3 direction);
	void  Rotate(XMFLOAT3 axis, float degrees);
	void  Position(XMFLOAT3& new_position);
	const XMFLOAT3& Position() const { return mPosition; }
	XMVECTOR& Rotation() { return XMQuaternionRotationMatrix(to(mView)); }
	void  Target(XMFLOAT3 new_target);
	const XMFLOAT3& Target() const { return mTarget; }
	const XMFLOAT3 Up() { return to(to(mUp) - to(mPosition)); }
	const XMFLOAT3 LookAtTarget() { return to(to(mTarget) - to(mPosition)); }
	const XMMATRIX View() { return XMMatrixTranspose(to(mView)); }

	// Set view frustum's angle
	void Angle(float angle);
	// Get view frustum's angle
	const float& Angle() const { return mAngle; }

	void NearestPlane(float nearest);
	void FarthestPlane(float farthest);

	const XMMATRIX Proj() { return XMMatrixTranspose(to(mProj)); }
	const XMFLOAT4X4 Ortho() { return to(XMMatrixTranspose(to(mOrtho))); }

	VectorPair getRay(float x, float y)
	{

		XMVECTOR unprojA = Unproject(x, y, 0.0f);
		XMVECTOR unprojB = Unproject(x, y, 1.0f);

		XMVECTOR dir = unprojB - unprojA;
		return std::make_pair(unprojA, unprojA + XMVector3ClampLength(dir, 999,1000));
	}
	

	XMFLOAT4X4  mView;		// View matrix
private:
	/*** Camera parameters ***/
	XMFLOAT3 mPosition;		// Camera's coordinates
	XMFLOAT3 mTarget;		// View target's coordinates
	XMFLOAT3 mUp;			// Camera's up vector end coordinates

	/*** Projection parameters ***/
	float mAngle;			// Angle of view frustum
	float mClientWidth;		// Window's width
	float mClientHeight;	// Window's height
	float mNearest;			// Nearest view frustum plane
	float mFarthest;		// Farthest view frustum plane

	XMFLOAT4X4	mProj;		// Projection matrix
	XMFLOAT4X4	mOrtho;		// Ortho matrix for drawing without tranformation

	XMVECTOR Unproject(const float& px, const float& py, const float& pz)
	{
		XMFLOAT2 res = {mClientWidth, mClientHeight};

		XMVECTOR q = XMQuaternionRotationMatrix(to(mView));

		XMVECTOR coords = XMVector3Unproject(XMVectorSet(px, py / tan(XMConvertToRadians(60.0f)), pz, 0.0f),
			0.0f, 0.0f,
			res.x, res.y,
			0, 1,
			to(mProj), to(mView), XMMatrixIdentity());

		return coords;
	}

	XMVECTOR Project(XMVECTOR v, XMMATRIX& world)
	{
		XMFLOAT2 res = { mClientWidth, mClientHeight };
		XMVECTOR coords = XMVector3Project(v,
			0.0f, 0.0f,
			res.x, res.y,
			0, 1,
			to(mProj), to(mView), world);

		return coords;
	}
};

