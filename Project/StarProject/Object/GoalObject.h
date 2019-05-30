#pragma once
#include "Obstacle.h"
class GoalObject :
	public Obstacle
{
public:
	GoalObject(const std::shared_ptr<Camera>& camera, int x, int y);
	~GoalObject();
};

