#include "Diodon.h"

Diodon::Diodon()
{
	auto pos  = Vector2(300, 300);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel  = Vector2();

	Swim();
}

Diodon::~Diodon()
{
}

void Diodon::Swim()
{
	updater = &Diodon::SwimUpdate;
}

void Diodon::Swell()
{
	updater = &Diodon::SwellUpdate;
}

void Diodon::Die()
{
	updater = &Diodon::DieUpdate;
}

void Diodon::SwimUpdate()
{
}

void Diodon::SwellUpdate()
{
}

void Diodon::DieUpdate()
{
}

void Diodon::Draw()
{
	DxLib::DrawBox(enemy._rect.Left(), enemy._rect.Top(),
				   enemy._rect.Right(), enemy._rect.Bottom(), color, false);

	DxLib::DrawString(0, 40, "ÊØ¾ÝÎÞÝ‚Ì•\Ž¦", 0xffffff);
}

void Diodon::Update()
{
	(this->*updater)();

	enemy._pos += _vel;
	enemy._rect = Rect(enemy._pos, enemy._size);
}

EnemyInfo Diodon::GetInfo()
{
	return enemy;
}

void Diodon::ChangeColor()
{
	color = 0x888800;
}

void Diodon::ResetColor()
{
	color = 0xffff00;
}
