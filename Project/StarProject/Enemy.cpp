#include "Enemy.h"
#include <DxLib.h>

Enemy::Enemy()
{
	/// e¸×½‚É‚µ‚Ä‚¢‚é‚Ì‚ÅA‚±‚±‚Ì‰Šú‰»‚ÍŒã‚ÅÁ‚·‚©‚à
	auto pos  = Vector2(400, 400);
	auto size = Size(50, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
}

Enemy::~Enemy()
{
}

void Enemy::Draw()
{
	DxLib::DrawBox(enemy._rect.Left(), enemy._rect.Top(),
				   enemy._rect.Right(), enemy._rect.Bottom(), color, false);
}

void Enemy::Update()
{
}

EnemyInfo Enemy::GetInfo()
{
	return enemy;
}

void Enemy::ChangeColor()
{
	color = 0xff0000;
}

void Enemy::ResetColor()
{
	color = 0xff00ff;
}
