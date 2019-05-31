#include "Enemy.h"
#include "../Camera.h"
#include "../ResourceManager.h"

Enemy::Enemy(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p) :_camera(c), _player(p)
{
	damageSE = ResourceManager::GetInstance().LoadSound("damage.mp3");
}

Enemy::~Enemy()
{
}
