#pragma once
#include "Geometry.h"

class Collision
{
public:
	Collision();
	~Collision();

	void Update();
	bool SquareToSquare(Position2& posA, Size& sizeA, Position2& posB, Size& sizeB);
	bool SquareToTriangle(Position2 posA);
};

