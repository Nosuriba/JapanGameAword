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
	void (DestroyableObject::*updater)(const Input& p);
	void NormalUpdate(const Input& p);
	void BreakUpdate(const Input& p);
	std::shared_ptr<Camera>& _camera;

	int destroyactimg[4];
	int destroylevel2actimg[4];

	int flame;
	int color;

public:
	DestroyableObject(std::shared_ptr<Camera>& camera, int x, int y, int level);
	~DestroyableObject();

	void Draw();
	void Update(const Input& p);

	void Break();

	ObjectInfo GetInfo();
};

