#pragma once
#include "Obstacle.h"
class GoalObject :
	public Obstacle
{
private:
	int goalimg;

public:
	GoalObject(const std::shared_ptr<Camera>& camera, int x, int y);
	~GoalObject();
	void Draw();
};

