#include "DestroyableObject.h"
#include "../Camera.h"
#include "../ResourceManager.h"


DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32,32);
	auto rect = Rect(pos, size);
	auto level = 1;

	obj = ObjectInfo(pos, size, rect, level, false);

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

	/*DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, 0x00ff00, false);*/
}

void DestroyableObject::Update()
{
	
}

void DestroyableObject::Break()
{
	obj._breakflag = true;
}

ObjectInfo DestroyableObject::GetInfo()
{
	return obj;
}
