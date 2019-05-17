#include "DestroyableObject.h"
#include "../Camera.h"
#include "../ResourceManager.h"


DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(16,16);
	auto rect = Rect(pos, size);
	auto color = 0xff00ff;

	obj = ObjectInfo(pos, size, rect, color);

	destroyimg = ResourceManager::GetInstance().LoadImg("../img/destroy.png");
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
