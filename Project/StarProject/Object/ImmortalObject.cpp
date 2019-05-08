#include "ImmortalObject.h"
#include "../Camera.h"
#include <DxLib.h>


ImmortalObject::ImmortalObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
	auto pos = Position2();
	auto size = Size();
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);

	immortalimg = DxLib::LoadGraph("../img/immortal.png");
}


ImmortalObject::~ImmortalObject()
{
}

void ImmortalObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (auto &immortalInfo : _immortalInfo) {
		DxLib::DrawExtendGraph(immortalInfo._rect.Left() - camera.x, immortalInfo._rect.Top() - camera.y,
			immortalInfo._rect.Right() - camera.x, immortalInfo._rect.Bottom() - camera.y, immortalimg, true);

		DxLib::DrawBox(immortalInfo._rect.Left() - camera.x, immortalInfo._rect.Top() - camera.y,
			immortalInfo._rect.Right() - camera.x, immortalInfo._rect.Bottom() - camera.y, 0xff00ff, false);
	}
}

void ImmortalObject::Update()
{
}

void ImmortalObject::ObjCreate(const Position2 & _pos, const Size & _size)
{
	auto pos = _pos;
	auto size = _size;
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);
	_immortalInfo.push_back(object);
}

std::vector<ObjectInfo> ImmortalObject::GetInfo()
{
	return _immortalInfo;
}
