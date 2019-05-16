#include "Boss.h"

Boss::Boss(std::shared_ptr<Camera>& camera) : _camera(camera)
{
}

Boss::~Boss()
{
}

void Boss::Draw()
{
}

void Boss::Update()
{
}

BossInfo Boss::GetInfo()
{
	return boss;
}
