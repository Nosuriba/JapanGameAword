#include "ImmortalObject.h"
#include "../Camera.h"
#include <DxLib.h>
#include "../ResourceManager.h"


ImmortalObject::ImmortalObject(std::shared_ptr<Camera>& camera, float& x, float& y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(16,16);
	auto rect = Rect(pos, size);
	auto color = 0xff00ff;

	obj = ObjectInfo(pos, size, rect, color);

	immortalimg = ResourceManager::GetInstance().LoadImg("../img/immortal.png");
}


ImmortalObject::~ImmortalObject()
{
}

void ImmortalObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, immortalimg, true);

	DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, obj._color, false);

}

void ImmortalObject::Update()
{
}

void ImmortalObject::Break()
{
	obj._color = 0x000000;
}

void ImmortalObject::ResetColor()
{
	obj._color = 0xff00ff;
}

ObjectInfo ImmortalObject::GetInfo()
{
	return obj;
}
