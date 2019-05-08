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
	std::vector<ObjectInfo> _predatory;

	int predatoryimg;

public:
	PredatoryObject(std::shared_ptr<Camera>& camera);
	~PredatoryObject();

	void Draw();
	void Update();
	void ObjCreate(const Position2& pos, const Size& size);
	std::vector<ObjectInfo> GetInfo();
};

