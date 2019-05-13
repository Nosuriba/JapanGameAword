#include "Camera.h"
#include <DxLib.h>
#include "Game.h"


Camera::Camera()
{
	_img = LoadGraph("../img/çªïl.png");

 	//GetGraphSizeF(_img, &_range.x, &_range.y);

	_range = Vector2(3000, 3000);
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
	auto size = Game::GetInstance().GetScreenSize();
	DrawExtendGraph(0 - _correction.x, 0 - _correction.y, _img, 
		size.x - _correction.x, size.y - _correction.y, true);
}

const Vector2 Camera::CameraCorrection() const
{
	return _correction;
}
