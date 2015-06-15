#pragma once
#include "ResourceManager.h"
#include "Entity.h"
class Player
{
public:
	Player(ResourceManager *rm);

	void RotateHead(XMVECTOR focus);
	Entity* head;
	Entity* body;
	~Player();
};
