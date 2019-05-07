#include "Obstacle.h"
#include "Camera.h"

Obstacle::Obstacle(std::shared_ptr<Camera>& camera):_camera(camera)
{
	auto pos = Vector2();
	auto size = Size();
	auto obj = ObjectInfo(pos, size);
}

Obstacle::~Obstacle()
{
}

void Obstacle::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (auto &obj : _obj) {
		DxLib::DrawBox(obj._pos.x - obj._size.width / 2 - camera.x, obj._pos.y - obj._size.height / 2 - camera.y,
			obj._pos.x + obj._size.width / 2 - camera.x, obj._pos.y + obj._size.height / 2 - camera.y,0xff00ff,true);
	}
}

void Obstacle::Update()
{
}

void Obstacle::ObjCreate(const Vector2 & _pos, const Size & _size)
{
	auto pos = _pos;
	auto size = _size;
	auto obj = ObjectInfo(pos, size);
	_obj.emplace_back(obj);
}

std::vector<ObjectInfo> Obstacle::GetObjectInfo()
{
	return _obj;
}
