#pragma once
#include "Obstacle.h"
#include <memory>
#include <vector>
#include "../Processing/Geometry.h"

class PredatoryObject :
	public Obstacle
{
private:
	std::shared_ptr<Camera>& _camera;
	ObjectInfo _predatory;

	int predatoryimg;

public:
	PredatoryObject(std::shared_ptr<Camera>& camera, float& x, float& y);
	~PredatoryObject();

	void Draw();
	void Update();
	void Break();
	ObjectInfo GetInfo();
};

