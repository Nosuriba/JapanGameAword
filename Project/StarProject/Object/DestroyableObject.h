#pragma once
#include "Obstacle.h"
#include <array>

class DestroyableObject :
	public Obstacle
{
private:
	void (DestroyableObject::*updater)();
	void NormalUpdate();
	void BreakUpdate();

	int _img1;
	int _img2;
	int breakSE;
	std::array<int, 2> _imgs;

	int flame;
	int color;

public:
	DestroyableObject(const std::shared_ptr<Camera>& camera, int x, int y, int level);
	~DestroyableObject();

	void Draw();
	void Update();

	void Break();

	ObjectInfo GetInfo();
};

