#pragma once
#include "Boss.h"

class Crab :
	public Boss
{
private:

	std::shared_ptr<Camera>& _camera;

	void (Crab::*_updater)();

	Vector2 _vel;
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void Update();
	BossInfo GetInfo();
};
