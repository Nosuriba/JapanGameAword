#include "PredatoryObject.h"
#include "../Camera.h"
#include <DxLib.h>
#include "../ResourceManager.h"

const int offset(60);

PredatoryObject::PredatoryObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32 + offset,32 + offset);
	auto rect = Rect(pos, size);

	obj = ObjectInfo(pos, size, rect, 0, false, false);

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

	SetDrawBlendMode(DX_BLENDMODE_ADD, abs(changecolor / 5 % 40 - 20));

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, predatoryimg, true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, abs(changecolor / 5 % 40 - 20));

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, predatoryimg, true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, abs(changecolor / 5 % 40 - 20));

	DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, predatoryimg, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/*DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, 0x0ff00, false);*/
}

void PredatoryObject::Update()
{
	changecolor++;
}

void PredatoryObject::Break()
{
	obj._breakflag = true;
}

void PredatoryObject::Predatory()
{
	obj._hitflag = true;
}

ObjectInfo PredatoryObject::GetInfo()
{
	return obj;
}
