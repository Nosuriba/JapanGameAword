#pragma once
#include "Obstacle.h"
#include <memory>

class PredatoryObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera;
public:
	PredatoryObject(std::shared_ptr<Camera>& camera);
	~PredatoryObject();
};

