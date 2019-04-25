#pragma once
#include <array>
#include "Geometry.h"

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
	Star star;

	Vector2 vel;

	char Buf[256];

public:
	Player();
	~Player();

	void Update();
	void Draw();
};

