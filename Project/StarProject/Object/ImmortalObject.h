#pragma once
#include "Obstacle.h"
#include "../Processing/Geometry.h"

#include <memory>
#include <vector>

class Camera;

class ImmortalObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera; 
	std::vector<ObjectInfo> _immortalInfo;

	int immortalimg;

public:
	ImmortalObject(std::shared_ptr<Camera>& camera, int x, int y);
	~ImmortalObject();
	void Draw();
	void Update();

	ObjectInfo GetInfo();
};

