#include "Shot.h"
#include "Particle/Water.h"
constexpr float _shotSpeed = 3.0f;


Shot::Shot(const Vector2& p, const Vector2& v) :_pos(p), _vel(v), _isHit(false), _isEnd(false), _count(0)
{
	particle = std::make_unique<Water>();
}

Shot::~Shot()
{
}

void Shot::Update()
{
	++_count;
	_pos += _vel * _shotSpeed;
	_vel += _vel.Normalized();
}

void Shot::Draw()
{
	DrawCircle(_pos.x, _pos.y, 3, 0x00ffff, true);
}

void Shot::Hit()
{
	_isHit = true;
}

void Shot::End()
{
	_isEnd = true;
}
