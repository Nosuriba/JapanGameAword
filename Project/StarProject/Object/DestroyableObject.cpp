#include "DestroyableObject.h"
#include "../Camera.h"
#include "../ResourceManager.h"

const int offset = 60;

void DestroyableObject::NormalUpdate(const Input & p)
{
}

void DestroyableObject::BreakUpdate(const Input & p)
{
	auto camera = _camera->CameraCorrection();
	if (flame < 30) {
		Draw();
	}
	else if (flame >= 30) {
		obj._breakflag = true;
		_updater = &DestroyableObject::NormalUpdate;
	}
	flame++;
}

DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera),_camera(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32 + offset,32 + offset);
	auto rect = Rect(pos, size);
	auto level = 1;

	obj = ObjectInfo(pos, size, rect, level, false ,false);

	flame = 0;

	destroyimg = ResourceManager::GetInstance().LoadImg("../img/destroy.png");
	LoadDivGraph("../img/destroy.png", 4, 2, 2, 624 / 2, 624 / 2, destroyactimg);

	_updater = &DestroyableObject::NormalUpdate;
}

DestroyableObject::~DestroyableObject()
{
}

void DestroyableObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	/*DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, destroyimg, true);*/
	

	DxLib::DrawExtendGraph(
		obj._rect.Left() - camera.x,
		obj._rect.Top() - camera.y - (flame),
		obj._rect.Right() - obj._rect.size.width / 2- camera.x,
		obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y - (flame) ,
		destroyactimg[0], true);

	DxLib::DrawExtendGraph(
		obj._rect.Left() + obj._rect.size.width / 2 - camera.x + (flame),
		obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x + (flame), 
		obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y, 
		destroyactimg[1], true);

	DxLib::DrawExtendGraph(
		obj._rect.Left() - camera.x - (flame), 
		obj._rect.Top() - camera.y + obj._rect.size.height / 2,
		obj._rect.Right() - obj._rect.size.height / 2 -camera.x - (flame),
		obj._rect.Bottom() - camera.y, 
		destroyactimg[2], true);

	DxLib::DrawExtendGraph(
		obj._rect.Left() + obj._rect.size.width / 2 - camera.x,
		obj._rect.Top() + obj._rect.size.height / 2 - camera.y + (flame),
		obj._rect.Right() - camera.x, 
		obj._rect.Bottom() - camera.y + (flame), 
		destroyactimg[3], true);
}

void DestroyableObject::Update(const Input& p)
{
	(this->*_updater)(p);
}

void DestroyableObject::Break()
{
	flame = 0;
	obj._hitflag = true;
	_updater = &DestroyableObject::BreakUpdate;
}

ObjectInfo DestroyableObject::GetInfo()
{
	return obj;
}
