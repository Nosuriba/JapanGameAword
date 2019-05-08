#include "PredatoryObject.h"
#include "../Camera.h"
#include <DxLib.h>


PredatoryObject::PredatoryObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(400,300);
	auto size = Size(40,40);
	auto rect = Rect(pos, size);
	auto color = 0xff00ff;

	obj = ObjectInfo(pos, size, rect, color);

	predatoryimg = DxLib::LoadGraph("../img/predatory.png");
}

PredatoryObject::~PredatoryObject()
{
}

void PredatoryObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, predatoryimg, true);

	DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, obj._color, false);
}

void PredatoryObject::Update()
{
}

void PredatoryObject::Break()
{
	obj._color = 0x000000;
}

ObjectInfo PredatoryObject::GetInfo()
{
	return obj;
}
