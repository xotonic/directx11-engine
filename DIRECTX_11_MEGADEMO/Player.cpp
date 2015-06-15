#include "Player.h"

Player::Player(ResourceManager *rm)
{
	head = new Entity(rm, "head", "default", "armor_diffuse", "armor_normal");
	body = new Entity(rm, "body", "default", "armor_diffuse", "armor_normal");
}

void Player::RotateHead(XMVECTOR focus)
{
	focus = XMVectorSetY(focus, head->transform()->Position().y);

	head->transform()->Rotate({ 0, 1, 0 }, 0.1);
}

Player::~Player()
{
	delete head;
	delete body;
}