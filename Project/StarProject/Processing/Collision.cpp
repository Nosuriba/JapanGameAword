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

/// éOäpå`Ç∆Ãﬂ⁄≤‘∞ÇÃìñÇΩÇËîªíË(âºÇ≈çÏÇ¡ÇƒÇ›ÇΩ)
bool Collision::TriToTri(const std::vector<Leg>& _leg, const std::array<Vector2, 3>& _tri)
{
	Vector2 triA = _tri[0];
 	Vector2 triB = _tri[1];
	Vector2 triC = _tri[2];

	//for (int i = 0; i < _leg.size(); ++i)
	//{
	//	auto AB1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triA.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triA.x);
	//	auto AB2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triB.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triB.x);
	//	auto AB3 = (triA.x - triB.x) * (_leg[i].tip.y - triA.y) + (triA.y - triB.y) * (triA.x - _leg[i].tip.x);
	//	auto AB4 = (triA.x - triB.x) * (_leg[(i + 2) % 5].tip.y - triA.y) + (triA.y - triB.y) * (triA.x - _leg[(i + 2) % 5].tip.x);

	//	auto BC1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triB.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triB.x);
	//	auto BC2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triC.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triC.x);
	//	auto BC3 = (triB.x - triC.x) * (_leg[i].tip.y - triB.y) + (triB.y - triC.y) * (triB.x - _leg[i].tip.x);
	//	auto BC4 = (triB.x - triC.x) * (_leg[(i + 2) % 5].tip.y - triB.y) + (triB.y - triC.y) * (triB.x - _leg[(i + 2) % 5].tip.x);

	//	auto CA1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triC.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triC.x);
	//	auto CA2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (triA.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - triA.x);
	//	auto CA3 = (triC.x - triA.x) * (_leg[i].tip.y - triC.y) + (triC.y - triA.y) * (triC.x - _leg[i].tip.x);
	//	auto CA4 = (triC.x - triA.x) * (_leg[(i + 2) % 5].tip.y - triC.y) + (triC.y - triA.y) * (triC.x - _leg[(i + 2) % 5].tip.x);

	//	if ((AB1 * AB2 < 0) && (AB3 * AB4 < 0) ||
	//		(BC1 * BC2 < 0) && (BC3 * BC4 < 0) ||
	//		(CA1 * CA2 < 0) && (CA3 * CA4 < 0))
	//	{
	//		return true;
	//	}
	//}
	return false;
}

bool Collision::TriToSqr(const std::vector<Leg>& _leg, const Position2 &_pos, const Size &_size)
{
	Vector2 sqrA = Vector2(_pos.x - _size.width / 2, _pos.y - _size.height / 2);
	Vector2 sqrB = Vector2(_pos.x + _size.width / 2, _pos.y - _size.height / 2);
	Vector2 sqrC = Vector2(_pos.x + _size.width / 2, _pos.y + _size.height / 2);
	Vector2 sqrD = Vector2(_pos.x - _size.width / 2, _pos.y + _size.height / 2);

	for (int i = 0; i < _leg.size(); i++) {

		auto AB1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrA.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrA.x);
		auto AB2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrB.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrB.x);
		auto AB3 = (sqrA.x - sqrB.x) * (_leg[i].tip.y - sqrA.y) + (sqrA.y - sqrB.y) * (sqrA.x - _leg[i].tip.x);
		auto AB4 = (sqrA.x - sqrB.x) * (_leg[(i + 2) % 5].tip.y - sqrA.y) + (sqrA.y - sqrB.y) * (sqrA.x - _leg[(i + 2) % 5].tip.x);

		auto BC1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrB.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrB.x);
		auto BC2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrC.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrC.x);
		auto BC3 = (sqrB.x - sqrC.x) * (_leg[i].tip.y - sqrB.y) + (sqrB.y - sqrC.y) * (sqrB.x - _leg[i].tip.x);
		auto BC4 = (sqrB.x - sqrC.x) * (_leg[(i + 2) % 5].tip.y - sqrB.y) + (sqrB.y - sqrC.y) * (sqrB.x - _leg[(i + 2) % 5].tip.x);

		auto CD1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrC.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrC.x);
		auto CD2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrD.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrD.x);
		auto CD3 = (sqrC.x - sqrD.x) * (_leg[i].tip.y - sqrC.y) + (sqrC.y - sqrD.y) * (sqrC.x - _leg[i].tip.x);
		auto CD4 = (sqrC.x - sqrD.x) * (_leg[(i + 2) % 5].tip.y - sqrC.y) + (sqrC.y - sqrD.y) * (sqrC.x - _leg[(i + 2) % 5].tip.x);

		auto DA1 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrD.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrD.x);
		auto DA2 = (_leg[i].tip.x - _leg[(i + 2) % 5].tip.x) * (sqrA.y - _leg[i].tip.y) + (_leg[i].tip.y - _leg[(i + 2) % 5].tip.y) * (_leg[i].tip.x - sqrA.x);
		auto DA3 = (sqrD.x - sqrA.x) * (_leg[i].tip.y - sqrD.y) + (sqrD.y - sqrA.y) * (sqrD.x - _leg[i].tip.x);
		auto DA4 = (sqrD.x - sqrA.x) * (_leg[(i + 2) % 5].tip.y - sqrD.y) + (sqrD.y - sqrA.y) * (sqrD.x - _leg[(i + 2) % 5].tip.x);

		if (((AB1 * AB2 < 0) && (AB3 * AB4 < 0)) ||
			((BC1 * BC2 < 0) && (BC3 * BC4 < 0)) ||
			((CD1 * CD2 < 0) && (CD3 * CD4 < 0)) ||
			((DA1 * DA2 < 0) && (DA3 * DA4 < 0))) {
			return true;
		}
	}
	return false;
}

bool Collision::WaterToSqr(const Position2 & _posA, const Vector2 & _vec, const Rect& _rectB)
{
	//_posA = player,_vec = laser,_rectB = enemy;

	const int r = 30;

	auto _vecA = _vec;

	auto _vecB = _rectB.center - _posA;

	auto _t = Dot(_vecA.Normalized(), _vecB);

	auto _p = _posA + (_vecA.Normalized() * max(0,_t));


	auto termA = ((_rectB.Left() < _p.x) && (_rectB.Right() > _p.x) && (_rectB.Top() - r < _p.y) && (_rectB.Bottom() + r > _p.y));

	auto termB = ((_rectB.Top() < _p.y) && (_rectB.Bottom() > _p.y) && (_rectB.Right() - r < _p.x) && (_rectB.Left() + r > _p.x));

	auto termC = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termD = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termE = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	auto termF = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	if (termA || termB || termC || termD || termE || termF) {
		return true;
	}
	return false;
}

bool Collision::WaterToSqr(const Position2 & _posA, const Vector2 & _vec, const float & size, const Rect & _rectB)
{
	const int r = 30;

	auto _vecA = _vec;

	auto _vecB = _rectB.center - _posA;

	auto _t = Dot(_vecA.Normalized(), _vecB);

	if (_t > size) {
		return false;
	}

	auto _p = _posA + (_vecA.Normalized() * max(0, _t));


	auto termA = ((_rectB.Left() < _p.x) && (_rectB.Right() > _p.x) && (_rectB.Top() - r < _p.y) && (_rectB.Bottom() + r > _p.y));

	auto termB = ((_rectB.Top() < _p.y) && (_rectB.Bottom() > _p.y) && (_rectB.Right() - r < _p.x) && (_rectB.Left() + r > _p.x));

	auto termC = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termD = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termE = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	auto termF = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	if (termA || termB || termC || termD || termE || termF) {
		return true;
	}
	return false;
}

bool Collision::CircleToSqr(const Position2 & _posA, const float& _r, const Rect & _rectB)
{
	auto termA = ((_rectB.Left() < _posA.x) && (_rectB.Right() > _posA.x) && (_rectB.Top() - _r < _posA.y) && (_rectB.Bottom() + _r > _posA.y));

	auto termB = ((_rectB.Top() < _posA.y) && (_rectB.Bottom() > _posA.y) && (_rectB.Right() - _r < _posA.x) && (_rectB.Left() + _r > _posA.x));

	auto termC = ((_rectB.Left() - _posA.x)*(_rectB.Left() - _posA.x) + (_rectB.Top() - _posA.y)*(_rectB.Top() - _posA.y) < _r*_r);

	auto termD = ((_rectB.Right() - _posA.x)*(_rectB.Right() - _posA.x) + (_rectB.Top() - _posA.y)*(_rectB.Top() - _posA.y) < _r*_r);

	auto termE = ((_rectB.Right() - _posA.x)*(_rectB.Right() - _posA.x) + (_rectB.Bottom() - _posA.y)*(_rectB.Bottom() - _posA.y) < _r*_r);

	auto termF = ((_rectB.Left() - _posA.x)*(_rectB.Left() - _posA.x) + (_rectB.Bottom() - _posA.y)*(_rectB.Bottom() - _posA.y) < _r*_r);

	if (termA || termB || termC || termD || termE || termF) {
		return true;
	}
	return false;
}
