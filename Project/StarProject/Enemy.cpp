#include "Enemy.h"
#include <DxLib.h>

Enemy::Enemy()
{
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
	DxLib::DrawBox(
		enemy._pos.x - enemy._size.width / 2,
		enemy._pos.y - enemy._size.height / 2,
		enemy._pos.x + enemy._size.width / 2,
		enemy._pos.y + enemy._size.height / 2,
		color, false);
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
