#include "Camera.h"
#include <DxLib.h>
#include "Game.h"

Camera::Camera()
{
	_pos	= Vector2();
	_target = Vector2();
	_range	= Vector2();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector2 & p)
{
	_target = p;
	_pos	= p;
}

void Camera::Update()
{
	auto& size = Game::GetInstance().GetScreenSize();

	auto v = _pos - _target;

	if (v.Magnitude() < 20) return;

	_pos = _target + v.Normalized() * (v.Magnitude() *0.9f);

	if (_pos.x < size.x / 2) _pos.x = size.x / 2;
	if (_pos.y < size.y / 2) _pos.y = size.y / 2;
	if (_pos.x > _range.x - size.x / 2) _pos.x = _range.x - size.x / 2;
	if (_pos.y > _range.y - size.y / 2) _pos.y = _range.y - size.y / 2;

	_correction = Vector2((_pos.x - size.x / 2), (_pos.y - size.y / 2));
}

void Camera::SetFocus(const Vector2 & p)
{
	_target = p;
}

const Vector2 Camera::CameraCorrection() const
{
	return _correction;
}

const Vector2 Camera::GetShadowPos(const float & h)
{
	auto l = h * tan(90 - 60);
	Vector2 s = Vector2(1, -1) * l;

	return s;
}

const Vector2 Camera::GetRange() const
{
	return _range;
}

const void Camera::SetRange(const Vector2& range)
{
	_range = range;
}
