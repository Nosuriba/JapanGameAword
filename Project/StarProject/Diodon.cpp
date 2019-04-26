#include "Diodon.h"

Diodon::Diodon()
{
	auto pos  = Vector2(300, 300);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
}

Diodon::~Diodon()
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
}

EnemyInfo Diodon::GetInfo()
{
	return enemy;
}

void Diodon::ChangeColor()
{
	color = 0x00ff00;
}

void Diodon::ResetColor()
{
	color = 0xffff00;
}
