#include "Enemy.h"
#include "../Camera.h"

Enemy::Enemy(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p) :_camera(c), _player(p)
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
	return _enemy;
}
