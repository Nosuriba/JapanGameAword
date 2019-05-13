#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"

#define LEG(x) _oct.legs[x]

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Enemy(camera), _camera(camera)
{
	_oct.center = Vector2(700, 300);
	_oct.root = _oct.center + Vector2(0, 30);
	_oct.r = 100;
	_oct.legs.resize(8);
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		LEG(i).tip.x = _oct.root.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _oct.r;
		LEG(i).tip.y = _oct.root.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _oct.r;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint[j] = _oct.root + LEG(i).tip.Normalized()*(_oct.r / LEG(i).T);
		}
	}

}

void Octopus::Die()
{
}

void Octopus::DieUpdate()
{
}

void Octopus::FootMove()
{
}

void Octopus::Draw()
{
}

void Octopus::Update()
{
}

EnemyInfo Octopus::GetInfo()
{
	return EnemyInfo();
}

shot_vector Octopus::GetShotInfo()
{
	return shot_vector();
}

void Octopus::CalEscapeDir(const Vector2 & vec)
{
}

void Octopus::ChangeShotColor(const int & num)
{
}

void Octopus::CalTrackVel(const Vector2 & pos, bool col)
{
}

Octopus::~Octopus()
{
}