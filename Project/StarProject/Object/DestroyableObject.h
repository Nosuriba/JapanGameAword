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
	void (DestroyableObject::*_updater)(const Input& p);
	void NormalUpdate(const Input& p);
	void BreakUpdate(const Input& p);
	std::shared_ptr<Camera>& _camera;

	int destroyimg;
	int destroyactimg[4];

	int flame;

public:
	DestroyableObject(std::shared_ptr<Camera>& camera, int x, int y);
	~DestroyableObject();

	void Draw();
	void Update(const Input& p);

	void Break();

	ObjectInfo GetInfo();
};

