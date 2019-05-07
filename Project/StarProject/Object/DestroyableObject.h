#pragma once
#include "Obstacle.h"
#include <memory>

class Camera;

class DestroyableObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera;
public:
	DestroyableObject(std::shared_ptr<Camera>& camera);
	~DestroyableObject();
};

