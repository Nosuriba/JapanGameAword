#include "Camera.h"
#include <DxLib.h>
#include "Game.h"


Camera::Camera()
{
 	//GetGraphSizeF(_img, &_range.x, &_range.y);

	_range = Vector2(2000, 1500);
}


Camera::~Camera()
{
}

void Camera::Update(const Vector2& p)
{
	auto size = Game::GetInstance().GetScreenSize();
	_pos = p;
	if (_pos.x < size.x / 2) _pos.x = size.x / 2;
	if (_pos.y < size.y / 2) _pos.y = size.y / 2;
	if (_pos.x > _range.x - size.x / 2) _pos.x = _range.x - size.x / 2;
	if (_pos.y > _range.y - size.y / 2) _pos.y = _range.y - size.y / 2;

	_correction = Vector2((_pos.x - size.x / 2), (_pos.y - size.y / 2));
}

void Camera::Draw()
{
}

const Vector2 Camera::CameraCorrection() const
{
	return _correction;
}

const Vector2 Camera::GetRange() const
{
	return _range;
}
