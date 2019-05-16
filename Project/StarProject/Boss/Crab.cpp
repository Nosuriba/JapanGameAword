#include "Crab.h"
#include "../Camera.h"

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	Neutral();
}

Crab::~Crab()
{
}

void Crab::Neutral()
{
	_updater = &Crab::NeutralUpdate;
}

void Crab::Shot()
{
	_updater = &Crab::ShotUpdate;
}

void Crab::Die()
{
	_updater = &Crab::DieUpdate;
}

void Crab::NeutralUpdate()
{
}

void Crab::ShotUpdate()
{
}

void Crab::DieUpdate()
{
}

void Crab::Draw()
{
	DxLib::DrawString(0, 0, "Crab Instance", 0x000000);

#ifdef _DEBUG
	DebugDraw();
#endif
}

void Crab::DebugDraw()
{
}

void Crab::Update()
{
	(this->*_updater)();

	boss._crab._prePos = boss._crab._pos;
	boss._crab._pos += _vel;
}

BossInfo Crab::GetInfo()
{
	return boss;
}
