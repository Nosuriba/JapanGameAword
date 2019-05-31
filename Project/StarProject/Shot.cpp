#include "Shot.h"
#include "Camera.h"
#include "Particle/Water.h"


Shot::Shot(const Vector2& p, const Vector2& v, const std::shared_ptr<Camera>&c) :_pos(p), _vel(v), _isHit(false), _isEnd(false), _count(0),_camera(c)
{
	particle = std::make_unique<Water>(_pos.x, _pos.y, 10, _camera);
	particle->SetRota(atan2f(_vel.Normalized().y, _vel.Normalized().x)* 180/ DX_PI_F);
	particle->SetPos(_pos.x, _pos.y);
	particle->Create();
}

Shot::~Shot()
{
}

void Shot::Update()
{
	++_count;
	_pos += _vel;
	_vel += _vel.Normalized();
}

void Shot::Draw()
{
	auto cc = _camera->CameraCorrection();
	DrawCircle(_pos.x-cc.x, _pos.y-cc.y, 3, 0xffffff, true);
	particle->Draw(0x8080ff);
}

void Shot::Hit()
{
	_isHit = true;
}

void Shot::End()
{
	_isEnd = true;
}
