#include "DestroyableObject.h"
#include "../Camera.h"


DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
	auto pos = Position2();
	auto size = Size();
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);

	destroyimg = DxLib::LoadGraph("../img/destroy.png");
}

DestroyableObject::~DestroyableObject()
{
}

void DestroyableObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (auto &destroy : _destroy) {
		DxLib::DrawExtendGraph(destroy._rect.Left() - camera.x, destroy._rect.Top() - camera.y,
			destroy._rect.Right() - camera.x, destroy._rect.Bottom() - camera.y, destroyimg, true);

		DxLib::DrawBox(destroy._rect.Left() - camera.x, destroy._rect.Top() - camera.y,
			destroy._rect.Right() - camera.x, destroy._rect.Bottom() - camera.y, 0xff00ff, false);
	}
}

void DestroyableObject::Update()
{
}

void DestroyableObject::ObjCreate(const Position2 & _pos, const Size & _size)
{
	auto pos = _pos;
	auto size = _size;
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);
	_destroy.push_back(object);
}

std::vector<ObjectInfo> DestroyableObject::GetInfo()
{
	return _destroy;
}
