#pragma once

#include "common.h"
#include "Transformable.h"

class Camera : public Transformable
{
public:
	Camera();
	Camera(const float x /*= 0.0f*/, const float y /*= 0.0f*/, const float z /*= 0.0f*/);
	void Translate(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f)
	{
		world *= XMMatrixTranslation(-x, -y, -z);
	}


};

