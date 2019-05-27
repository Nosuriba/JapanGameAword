#include "PredatoryObject.h"
#include "../Camera.h"
#include <DxLib.h>
#include "../ResourceManager.h"


PredatoryObject::PredatoryObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32,32);
	auto rect = Rect(pos, size);
	auto level = 1;

	obj = ObjectInfo(pos, size, rect, level, false, false);

	predatoryimg = ResourceManager::GetInstance().LoadImg("../img/predatory.png");
}

PredatoryObject::~PredatoryObject()
{
}

void PredatoryObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, predatoryimg, true);

	/*DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, 0x0ff00, false);*/
}

void PredatoryObject::Update()
{
}

void PredatoryObject::Break()
{
	obj._breakflag = true;
}

void PredatoryObject::Predatory()
{
	obj._predatoryflag = true;
}

ObjectInfo PredatoryObject::GetInfo()
{
	return obj;
}
