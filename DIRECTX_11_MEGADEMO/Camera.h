#pragma once

#include "common.h"


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
	void  Target(XMFLOAT3 new_target);
	const XMFLOAT3& Target() const { return mTarget; }
	const XMFLOAT3 Up() { return to(to(mUp) - to(mPosition)); }
	const XMFLOAT3 LookAtTarget() { return to(to(mTarget) - to(mPosition)); }
	const XMFLOAT4X4 View() { return to(XMMatrixTranspose(to(mView))); }

	// Set view frustum's angle
	void Angle(float angle);
	// Get view frustum's angle
	const float& Angle() const { return mAngle; }

	void NearestPlane(float nearest);
	void FarthestPlane(float farthest);

	const XMFLOAT4X4 Proj() { return to(XMMatrixTranspose(to(mProj))); }
	const XMFLOAT4X4 Ortho() { return to(XMMatrixTranspose(to(mOrtho))); }

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

	XMFLOAT4X4  mView;		// View matrix
	XMFLOAT4X4	mProj;		// Projection matrix
	XMFLOAT4X4	mOrtho;		// Ortho matrix for drawing without tranformation

};

