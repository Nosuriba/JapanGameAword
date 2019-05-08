#include "PredatoryObject.h"
#include "../Camera.h"
#include <DxLib.h>


PredatoryObject::PredatoryObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
	auto pos = Position2();
	auto size = Size();
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);

	predatoryimg = DxLib::LoadGraph("../img/predatory.png");
}

PredatoryObject::~PredatoryObject()
{
}

void PredatoryObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (auto &predatory : _predatory) {
		DxLib::DrawExtendGraph(predatory._rect.Left() - camera.x, predatory._rect.Top() - camera.y,
			predatory._rect.Right() - camera.x, predatory._rect.Bottom() - camera.y, predatoryimg, true);

		DxLib::DrawBox(predatory._rect.Left() - camera.x, predatory._rect.Top() - camera.y,
			predatory._rect.Right() - camera.x, predatory._rect.Bottom() - camera.y, 0xff00ff, false);
	}
}

void PredatoryObject::Update()
{
}

void PredatoryObject::ObjCreate(const Position2 & _pos, const Size & _size)
{
	auto pos = _pos;
	auto size = _size;
	auto rect = Rect(pos, size);

	auto object = ObjectInfo(pos, size, rect);
	_predatory.push_back(object);
}

std::vector<ObjectInfo> PredatoryObject::GetInfo()
{
	return _predatory;
}
