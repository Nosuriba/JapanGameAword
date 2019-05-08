#include "DestroyableObject.h"
#include "../Camera.h"


DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(400,400);
	auto size = Size(40,40);
	auto rect = Rect(pos, size);
	auto color = 0xff00ff;

	obj = ObjectInfo(pos, size, rect, color);

	destroyimg = DxLib::LoadGraph("../img/destroy.png");
}

DestroyableObject::~DestroyableObject()
{
}

void DestroyableObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, destroyimg, true);

	DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, obj._color, false);
}

void DestroyableObject::Update()
{
}

void DestroyableObject::Break()
{
	obj._color = 0x000000;
}

ObjectInfo DestroyableObject::GetInfo()
{
	return obj;
}
