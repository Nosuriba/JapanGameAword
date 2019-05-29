#include "Enemy.h"
#include "../Camera.h"

Enemy::Enemy(std::shared_ptr<Camera>& camera):_camera(camera)
{
}

Enemy::~Enemy()
{
}

void Enemy::Draw()
{
}

void Enemy::Update()
{
}

EnemyInfo Enemy::GetInfo()
{
	return enemy;
}

void Enemy::ShotDelete(const int& num)
{
}

void Enemy::CalEscapeDir(const Vector2& vec)
{
}

void Enemy::CalTrackVel(const Vector2 & pos)
{
}
