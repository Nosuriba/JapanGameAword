#pragma once
#include "Geometry.h"
#include <array>

class Collision
{
private:
	Vector2 Sub_Calculation(const Vector2& posA, const Vector2& posB);
	float Cross(const Vector2& posA, const Vector2& posB);
public:
	Collision();
	~Collision();
	bool TriToSqr(const std::array<Vector2, 5> &_vert, const Position2 &_pos, const Size &_size);
};

