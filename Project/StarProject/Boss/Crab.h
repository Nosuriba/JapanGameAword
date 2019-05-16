#pragma once
#include "Boss.h"

class Crab :
	public Boss
{
private:
	void Neutral();
	void Shot();
	void Die();

	void NeutralUpdate();
	void ShotUpdate();
	void DieUpdate();

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	Vector2 _vel;
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void DebugDraw();
	void Update();
	BossInfo GetInfo();
};
