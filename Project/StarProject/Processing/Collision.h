#pragma once
#include "Geometry.h"
#include <array>
#include <vector>
#include "../Player.h"

class Collision
{
private:
	Vector2 Sub_Calculation(const Vector2& posA, const Vector2& posB);
	float Cross(const Vector2& posA, const Vector2& posB);
public:
	Collision();
	~Collision();
	bool TriToTri(const std::vector<Leg>& _leg, const std::array<Vector2, 3>& _tri);
	bool TriToSqr(const std::vector<Leg>& _leg, const Position2 &_pos, const Size &_size);
	bool WaterToSqr(const Position2 & _posA, const Vector2 & _vec, const Rect& _rect);
	bool WaterToSqr(const Position2 & _posA, const Vector2 & _vec, const float& size ,const Rect& _rect);
	bool CircleToSqr(const Position2 & _posA, const float &_r, const Rect & _rectB);
};

