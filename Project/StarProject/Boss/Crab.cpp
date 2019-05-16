#include "Crab.h"
#include "../Camera.h"

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
}

Crab::~Crab()
{
}

void Crab::Draw()
{
	DxLib::DrawString(0, 0, "Crab Instance", 0x000000);
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
