#pragma once
#include "Obstacle.h"

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

