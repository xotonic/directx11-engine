#include "Tank.h"

Tank::Tank(Renderer &r)
{
	head = new Entity(r.resMgr, "head", "default", "empty_diffuse", "empty_normal");
	body = new Entity(r.resMgr, "body", "default", "empty_diffuse", "empty_normal");

	//head->transform()->Move({0.f, 1.f, 0.f});
}

Tank::~Tank()
{
	delete head;
	delete body;
}

void Tank::Draw(Renderer *r)
{
	r->DrawEntity(head);
	r->DrawEntity(body);
}

void Tank::MoveBody(XMVECTOR dir)
{
	body->transform()->Rotate({0,1,0},XMConvertToRadians(90));
	body->transform()->Move(to(XMVector3Normalize(dir)*speed));
}

void Tank::RotateHead(XMVECTOR focus)
{
	body->transform()->Rotate({ 0, 1, 0 }, XMConvertToRadians(90));
}
