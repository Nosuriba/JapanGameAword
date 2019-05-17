#pragma once
#include "Obstacle.h"
#include <memory>
#include <vector>
#include "../Processing/Geometry.h"

class Camera;

class DestroyableObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera;

	int destroyimg;

public:
	DestroyableObject(std::shared_ptr<Camera>& camera, float& x, float& y);
	~DestroyableObject();

	void Draw();
	void Update();

	void Break();

	ObjectInfo GetInfo();
};

