#include "Fish.h"



Fish::Fish()
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
}


Fish::~Fish()
{
}

void Fish::Draw()
{
	DxLib::DrawBox(enemy._rect.Left(), enemy._rect.Top(),
				   enemy._rect.Right(), enemy._rect.Bottom(), color, false);

	DxLib::DrawString(0, 0, "‹›‚Ì•\Ž¦", 0xffffff);
}

void Fish::Update()
{
}

EnemyInfo Fish::GetInfo()
{
	return enemy;
}

void Fish::ChangeColor()
{
	color = 0xaaaaaa;
}

void Fish::ResetColor()
{
	color = 0xfffff0;
}
