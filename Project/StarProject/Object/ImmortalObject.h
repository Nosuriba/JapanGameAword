#pragma once
#include "Obstacle.h"
#include <memory>

class Camera;

class ImmortalObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera;
public:
	ImmortalObject(std::shared_ptr<Camera>& camera);
	~ImmortalObject();
};

