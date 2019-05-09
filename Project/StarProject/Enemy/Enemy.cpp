#include "Enemy.h"
#include <DxLib.h>
#include "../Camera.h"

Enemy::Enemy(std::shared_ptr<Camera>& camera):_camera(camera)
{
	/// 親ｸﾗｽにしているので、ここの初期化は後で消すかも
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

void Enemy::CalTrackVel(const Vector2 & pos, bool col)
{
	/// 何も書かない
}
