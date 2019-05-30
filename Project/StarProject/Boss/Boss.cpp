#include "Boss.h"


Boss::Boss(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p) :_camera(c), _player(p)
{
}

Boss::~Boss()
{
}

bool Boss::GetDieFlag()
{
	return _isDie;
}

std::list<AttackInfo> Boss::GetAttackInfo()
{
	return at;
}

std::list<DamageInfo> Boss::GetDamageInfo()
{
	return da;
}

std::list<ShotInfo> Boss::GetShotInfo()
{
	return shot;
}


