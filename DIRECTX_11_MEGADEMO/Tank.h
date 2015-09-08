#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Renderer.h"
class Tank
{
public:
	Tank(Renderer& r);
	~Tank();

	float speed = 1.0f;
	void Draw(Renderer *r);
	void MoveBody(XMVECTOR dir);
	void RotateHead(XMVECTOR focus);
private:
	Entity *head;
	Entity *body;
};

