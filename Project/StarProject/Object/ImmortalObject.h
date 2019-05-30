#pragma once
#include "Obstacle.h"

class ImmortalObject :
	public Obstacle
{
private:
	//int immortalimg[9];
	int _img;

public:
	ImmortalObject(std::shared_ptr<Camera>& camera, int x, int y);
	~ImmortalObject();
	void Draw();
	void Update();

	ObjectInfo GetInfo();
};

