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
	std::vector<ObjectInfo> _destroy;

	int destroyimg;

public:
	DestroyableObject(std::shared_ptr<Camera>& camera);
	~DestroyableObject();

	void Draw();
	void Update();

	void ObjCreate(const Position2& _pos, const Size& _size);
	std::vector<ObjectInfo> GetInfo();
};

