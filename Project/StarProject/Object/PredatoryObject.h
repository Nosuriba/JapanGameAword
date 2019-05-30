#pragma once
#include "Obstacle.h"

class PredatoryObject :
	public Obstacle
{
private:

	int predatoryimg;
	int changecolor;

public:
	PredatoryObject(const std::shared_ptr<Camera>& camera, int x, int y);
	~PredatoryObject();

	void Draw();
	void Update();
	void Break();
	void Predatory();
	ObjectInfo GetInfo();
};

