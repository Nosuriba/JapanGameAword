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
	bool TriToTri(const std::array<Vector2, 5>& _vert, const std::array<Vector2, 3>& _tri);
	bool TriToSqr(const std::array<Vector2, 5> &_vert, const Position2 &_pos, const Size &_size);
	bool WaterToSqr(const Position2 & _posA, const Vector2 & _vec, const Rect& _rect);

};

