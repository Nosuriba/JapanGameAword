#include "Boss.h"
#include "../Camera.h"
#include "../Player.h"


Boss::Boss(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p) :_camera(c), _player(p)
{
}

Boss::~Boss()
{
}

std::list<AttackInfo> Boss::GetAttackInfo()
{
	return at;
}


