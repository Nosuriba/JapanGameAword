#pragma once
#include "Processing/Geometry.h"
#include <memory>
class Particle;
class Camera;
class Shot
{
private:
	const std::shared_ptr<Camera> _camera;
	Vector2 _pos;
	Vector2 _vel;

	std::unique_ptr<Particle> particle;
	int		_count;
	bool	_isHit;
	bool	_isEnd;

public:
	Shot(const Vector2& p, const Vector2& v, const std::shared_ptr<Camera>&c);
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

