#pragma once
#include <array>
#include <vector>
#include <memory>
#include "Geometry.h"

class Camera;

struct Star {
	float r;
	Vector2 center;
	std::array<Vector2, 5> vertexs;
	std::array<bool, 5> shot;
	int axis;

	Star() : r(10), center(0, 0) {}
	Star(int& inr, Vector2& p) :r(inr), center(p) {}
};

class Player
{
private:
	const std::shared_ptr<Camera>& _camera;

	Star star;

	Vector2 vel;

	char Buf[256];

public:
	Player(const std::shared_ptr<Camera>& c);
	~Player();

	void Update();
	void Draw();
	Star GetInfo();
	const std::vector<Vector2> GetShot();
	const std::array<Vector2, 5> debugShot();

};

