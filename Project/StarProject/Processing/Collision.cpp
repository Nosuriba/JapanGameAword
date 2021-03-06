#include "Collision.h"
#include <cmath>

const int cutnum = 4;

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

/// 三角形とﾌﾟﾚｲﾔｰの当たり判定(仮で作ってみた)
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

bool Collision::WaterToSqr(const Position2 & _posA1, const Position2 & _posA2, const Rect & _rectB)
{
	const int r = 30;

	auto _vecA = _posA2 - _posA1;

	auto _vecB = _rectB.center - _posA1;

	auto _t = Dot(_vecA.Normalized(), _vecB);

	auto _p = _posA1 + (_vecA.Normalized() * min(max(0, _t), _vecA.Magnitude()));

	if ((_p - _rectB.center).Magnitude() <= _rectB.size.width / 2)
	{
		return true;
	}
	return false;

	auto termA = ((_rectB.Left() < _p.x) && (_rectB.Right() > _p.x) && (_rectB.Top() - r < _p.y) && (_rectB.Bottom() + r > _p.y));

	auto termB = ((_rectB.Top() < _p.y) && (_rectB.Bottom() > _p.y) && (_rectB.Right() - r < _p.x) && (_rectB.Left() + r > _p.x));

	auto termC = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termD = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Top() - _p.y)*(_rectB.Top() - _p.y) < r*r);

	auto termE = ((_rectB.Right() - _p.x)*(_rectB.Right() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	auto termF = ((_rectB.Left() - _p.x)*(_rectB.Left() - _p.x) + (_rectB.Bottom() - _p.y)*(_rectB.Bottom() - _p.y) < r*r);

	if (termA || termB || termC || termD || termE || termF) 
	{
		return true;
	}
	return false;
}

bool Collision::WaterToCircle(const Position2 & _posA1, const Position2 & _posA2, const Position2 & _posB, const float & size)
{
	auto _vecA = _posA2 - _posA1;

	auto _vecB = _posB - _posA1;

	auto _t = Dot(_vecA.Normalized(), _vecB);

	auto _p = _posA1 + (_vecA.Normalized() * min(max(0, _t), _vecA.Magnitude()));

	if ((_p - _posB).Magnitude() <= size) 
	{
		return true;
	}
	return false;
}

bool Collision::WaterToCircle(const Position2 & _posA1, const Position2 & _posA2, const Rect & rect)
{
	auto r = rect.size.width / 2;

	auto _vecA = _posA2 - _posA1;

	auto _vecB = rect.center - _posA1;

	auto _t = Dot(_vecA.Normalized(), _vecB);

	auto _p = _posA1 + (_vecA.Normalized() * min(max(0, _t), _vecA.Magnitude()));

	if ((_p - rect.center).Magnitude() <= r) {
		return true;
	}
	return false;
}

bool Collision::CircleToSqr(const Position2 & _posA, const float& _r, const Rect & _rectB)
{
	auto termA = ((_rectB.Left() < _posA.x) &&	(_rectB.Right() > _posA.x)	&&	(_rectB.Top() - _r < _posA.y)	&&	(_rectB.Bottom() + _r > _posA.y));

	auto termB = ((_rectB.Top() < _posA.y)	&&	(_rectB.Bottom() > _posA.y) &&	(_rectB.Right() - _r < _posA.x)	&&	(_rectB.Left() + _r > _posA.x));

	auto termC = ((_rectB.Left() - _posA.x)	*	(_rectB.Left() - _posA.x)	+	(_rectB.Top() - _posA.y)		*	(_rectB.Top() - _posA.y) < _r*_r);

	auto termD = ((_rectB.Right() - _posA.x)*	(_rectB.Right() - _posA.x)	+	(_rectB.Top() - _posA.y)		*	(_rectB.Top() - _posA.y) < _r*_r);

	auto termE = ((_rectB.Right() - _posA.x)*	(_rectB.Right() - _posA.x)	+	(_rectB.Bottom() - _posA.y)		*	(_rectB.Bottom() - _posA.y) < _r*_r);

	auto termF = ((_rectB.Left() - _posA.x)	*	(_rectB.Left() - _posA.x)	+	(_rectB.Bottom() - _posA.y)		*	(_rectB.Bottom() - _posA.y) < _r*_r);

	if (termA || termB || termC || termD || termE || termF) 
	{
		return true;
	}
	return false;
}

bool Collision::CircleToCircle(const Position2 & _posA, const float & _rA, const std::array<Vector2, 3>& _tri)
{
	//三角形の中心点
	auto _posB = (_tri[0] + _tri[1] + _tri[2]) / 3;

	auto vecA = _posA - _posB;

	auto _rB = _tri[0] - _posB;

	if (vecA.Magnitude() <= _rA + _rB.Magnitude()) {
		return true;
	}

	return false;
}

bool Collision::CircleToCircle(const Position2 & _posA, const float & _rA, const Position2 & _posB, const float & _rB)
{
	auto vecA = _posA - _posB;

	if (vecA.Magnitude() <= _rA + _rB) {
		return true;
	}

	return false;
}

int Collision::BitSeparate(const int & n)
{
	int num = n;
	num = (num | (num << 8)) & 0x00ff00ff;
	num = (num | (num << 4)) & 0x0f0f0f0f;
	num = (num | (num << 2)) & 0x33333333;
	num = (num | (num << 1)) & 0x55555555;

	return num;
}

int Collision::CalCulation(const Rect & rect)
{
	int sizex, sizey;
	GetDrawScreenSize(&sizex,& sizey);

	auto ltpos = Vector2(rect.Left(), rect.Top());			//左上座標
	auto rbpos = Vector2(rect.Right(), rect.Bottom());		//右下座標

	//分割した時のサイズに
	auto cutsize = Vector2(sizex / cutnum, sizey / cutnum);

	//左上座標の空間を計算
	int ltx = ltpos.x / cutsize.x;
	int lty = ltpos.y / cutsize.y;

	//間に0を入れていって六桁の二進数にする(その後の計算のため)
	ltx = BitSeparate(ltx);
	lty = BitSeparate(lty);

	//y座標を１ビット左シフト
	lty = lty << 1;

	//x座標とy座標をOR演算
	auto lt = ltx | lty;


	//右下座標の空間を計算
	int rbx = rbpos.x / cutsize.x;
	int rby = rbpos.y / cutsize.y;

	//間に0を入れていって六桁の二進数にする(その後の計算のため)
	rbx = BitSeparate(rbx);
	rby = BitSeparate(rby);

	//y座標を１ビット左シフト
	rby = rby << 1;

	//x座標とy座標をOR演算
	auto rb = rbx | rby;

	//左上空間の番号と左下空間の番号をXOR演算
	int ltrb = lt ^ rb;

	return ltrb;
	int spaceNum = 0;
	int i = 0;
	int shift = 0;

	//空間を特定
	while (ltrb != 0) {
		if ((ltrb & 0x3) != 0) {
			//空間シフト数
			spaceNum = (i + 1);
			shift = spaceNum * 2;
		}

		//2bitシフトさせて再チェック
		ltrb >>= 2;
		i++;
	}

	rb = rb >> shift;
}

Vector2 Collision::GetParentCal(const int& spaceNum, const int& rb)
{
	auto listnum = (rb - 1) / 4;

	auto spacenum = spaceNum + 1;

	return Vector2(spacenum, listnum);
}

Vector2 Collision::Pushback(const Star& star, const Rect& rect)
{
	auto v = star.center - rect.center;

	auto b = min(max((star.r + rect.size.width / 2) - v.Magnitude(), 0), star.r);

	return v.Normalized() * b;
}

Vector2 Collision::Pushback(const Vector2 & pos, const float r, const Rect & rect)
{
	auto v = pos - rect.center;
	auto b = (r + rect.size.width / 2) - v.Magnitude();
	
	return v.Normalized() * b;
}
