#pragma once
#include "Processing/Geometry.h"
class Particle;
class Shot
{
private:
	std::unique_ptr<Particle> particle;

	Vector2 _pos;
	Vector2 _vel;

	int		_count;
	bool	_isHit;
	bool	_isEnd;

public:
	Shot(const Vector2& p, const Vector2& v);
	~Shot();

	void Update();
	void Draw();

	void Hit();
	void End();

	const Vector2 GetPos() const { return _pos; }
	const bool HitCheck() const { return _isHit; }
	const bool EndCheck() const { return _isEnd; }
	const int CntCheck() const { return _count; }
};

