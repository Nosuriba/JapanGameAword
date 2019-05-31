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

	//int destroyactimg[4];
	//int destroylevel2actimg[4];

	int _img1;
	int _img2;
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

