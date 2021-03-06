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
	bool WaterToSqr(const Position2 & _posA1, const Position2 & _posA2, const Rect & _rectB);
	bool WaterToCircle(const Position2 & _posA1, const Position2 & _posA2, const Position2 & _posB, const float & size);
	bool WaterToCircle(const Position2 & _posA1, const Position2 & _posA2, const Rect& rect);
	bool CircleToSqr(const Position2 & _posA, const float &_r, const Rect & _rectB);
	bool CircleToCircle(const Position2 & _posA, const float & _rA, const std::array<Vector2, 3>& _tri);
	bool CircleToCircle(const Position2 & _posA, const float & _rA, const Position2 & _posB, const float & _rb);

	int BitSeparate(const int& num);

	int CalCulation(const Rect& rect);
	Vector2 GetParentCal(const int& spaceNum, const int& rb);
	Vector2 Pushback(const Star& star, const Rect& rect);
	Vector2 Pushback(const Vector2& pos, const float r, const Rect& rect);
};

