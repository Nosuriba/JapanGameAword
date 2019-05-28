#include "ImmortalObject.h"
#include "../Camera.h"
#include <DxLib.h>
#include "../ResourceManager.h"


ImmortalObject::ImmortalObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32,32);
	auto rect = Rect(pos, size);
	auto level = 1;

	obj = ObjectInfo(pos, size, rect, level, false);

	LoadDivGraph("../img/immortal.png", 9, 9, 1, 1357 / 9, 136, immortalimg);
}


ImmortalObject::~ImmortalObject()
{
}

void ImmortalObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, immortalimg[0], true);

	/*DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, 0x00ff00, false);*/

}

void ImmortalObject::Update()
{
}

ObjectInfo ImmortalObject::GetInfo()
{
	return obj;
}
