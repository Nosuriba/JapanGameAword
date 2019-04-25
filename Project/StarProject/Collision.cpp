#include "Collision.h"


Vector2 Collision::Sub_Calculation(const Vector2 & posA, const Vector2 & posB)
{
	Vector2 tmp;

	tmp.x = posA.x - posB.x;
	tmp.y = posA.y - posB.y;

	return tmp;
}

float Collision::Cross(const Vector2 & posA, const Vector2 & posB)
{
	return posA.x * posB.y - posA.y * posB.x;
}

Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::TriToSqr(const std::array<Vector2, 5> &_vert, const Position2 &_pos, const Size &_size)
{
	Vector2 sqrA = Vector2(_pos.x - _size.width / 2, _pos.y - _size.height / 2);
	Vector2 sqrB = Vector2(_pos.x + _size.width / 2, _pos.y - _size.height / 2);
	Vector2 sqrC = Vector2(_pos.x + _size.width / 2, _pos.y + _size.height / 2);
	Vector2 sqrD = Vector2(_pos.x - _size.width / 2, _pos.y + _size.height / 2);

	for (int i = 0; i < _vert.size(); i++) {

		auto AB1 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrA.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrA.x);
		auto AB2 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrB.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrB.x);
		auto AB3 = (sqrA.x - sqrB.x) * (_vert[i].y - sqrA.y) + (sqrA.y - sqrB.y) * (sqrA.x - _vert[i].x);
		auto AB4 = (sqrA.x - sqrB.x) * (_vert[(i + 2) % 5].x - sqrA.y) + (sqrA.y - sqrB.y) * (sqrA.x - _vert[(i + 2) % 5].x);

		auto BC1 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrB.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrB.x);
		auto BC2 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrC.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrC.x);
		auto BC3 = (sqrB.x - sqrC.x) * (_vert[i].y - sqrB.y) + (sqrB.y - sqrC.y) * (sqrB.x - _vert[i].x);
		auto BC4 = (sqrB.x - sqrC.x) * (_vert[(i + 2) % 5].x - sqrB.y) + (sqrB.y - sqrC.y) * (sqrB.x - _vert[(i + 2) % 5].x);

		auto CD1 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrC.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrC.x);
		auto CD2 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrD.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrD.x);
		auto CD3 = (sqrC.x - sqrD.x) * (_vert[i].y - sqrC.y) + (sqrC.y - sqrD.y) * (sqrC.x - _vert[i].x);
		auto CD4 = (sqrC.x - sqrD.x) * (_vert[(i + 2) % 5].x - sqrC.y) + (sqrC.y - sqrD.y) * (sqrC.x - _vert[(i + 2) % 5].x);

		auto DA1 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrD.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrD.x);
		auto DA2 = (_vert[i].x - _vert[(i + 2) % 5].x) * (sqrA.y - _vert[i].y) + (_vert[i].y - _vert[(i + 2) % 5].y) * (_vert[i].x - sqrA.x);
		auto DA3 = (sqrD.x - sqrA.x) * (_vert[i].y - sqrD.y) + (sqrD.y - sqrA.y) * (sqrD.x - _vert[i].x);
		auto DA4 = (sqrD.x - sqrA.x) * (_vert[(i + 2) % 5].x - sqrD.y) + (sqrD.y - sqrA.y) * (sqrD.x - _vert[(i + 2) % 5].x);

		if (((AB1 * AB2 < 0) && (AB3 * AB4 < 0)) ||
			((BC1 * BC2 < 0) && (BC3 * BC4 < 0)) ||
			((CD1 * CD2 < 0) && (CD3 * CD4 < 0)) ||
			((DA1 * DA2 < 0) && (DA3 * DA4 < 0))) {
			return true;
		}
	}
	return false;
}
