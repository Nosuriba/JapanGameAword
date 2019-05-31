#include "DestroyableObject.h"

#include "../ResourceManager.h"

const int offset = 15;

void DestroyableObject::NormalUpdate()
{
}

void DestroyableObject::BreakUpdate()
{
	if (flame >= 30) {
		obj._breakflag = true;
		updater = &DestroyableObject::NormalUpdate;
	}
	flame++;
}

DestroyableObject::DestroyableObject(const std::shared_ptr<Camera>& camera, int x, int y, int level):Obstacle(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32, 32);
	auto rect = Rect(pos, size);
	auto _level = level;

	obj = ObjectInfo(pos, size, rect, _level, false ,false);

	flame = 0;
	breakSE = ResourceManager::GetInstance().LoadSound("../Sound/sango.mp3");
	_imgs[0] = ResourceManager::GetInstance().LoadImg("../img/destroy.png");
	_imgs[1] = ResourceManager::GetInstance().LoadImg("../img/destroylevel2.png");

	updater = &DestroyableObject::NormalUpdate;
}

DestroyableObject::~DestroyableObject()
{
}

void DestroyableObject::Draw()
{
	auto camera = _camera->CameraCorrection();
	
	{
		// ¶ã
		DrawRectExtendGraph(
			obj._rect.Left()	- camera.x - offset,
			obj._rect.Top()		- camera.y - (flame) - offset,
			obj._rect.Right()	- obj._rect.size.width / 2	- camera.x,
			obj._rect.Bottom()	- obj._rect.size.height / 2 - camera.y - (flame),
			0, 0, 312, 312, _imgs[obj._level - 1], true
		);
		// ‰Eã
		DrawRectExtendGraph(
			obj._rect.Left()	+ obj._rect.size.width / 2 - camera.x + (flame),
			obj._rect.Top()		- camera.y - offset,
			obj._rect.Right()	- camera.x + (flame) + offset,
			obj._rect.Bottom()	- obj._rect.size.height / 2 - camera.y,
			312, 0, 312, 312, _imgs[obj._level - 1], true
		);
		// ¶‰º
		DrawRectExtendGraph(
			obj._rect.Left()	- camera.x - (flame) - offset,
			obj._rect.Top()		- camera.y + obj._rect.size.height / 2,
			obj._rect.Right()	- obj._rect.size.height / 2 - camera.x - (flame),
			obj._rect.Bottom()	- camera.y + offset,
			0, 312, 312, 312, _imgs[obj._level - 1], true
		);
		// ‰E‰º
		DrawRectExtendGraph(
			obj._rect.Left()	+ obj._rect.size.width / 2 - camera.x,
			obj._rect.Top()		+ obj._rect.size.height / 2 - camera.y + (flame),
			obj._rect.Right()	- camera.x + offset,
			obj._rect.Bottom()	- camera.y + (flame) + offset,
			312, 312, 312, 312, _imgs[obj._level - 1], true
		);
	}
}

void DestroyableObject::Update()
{
	(this->*updater)();
}

void DestroyableObject::Break()
{
	flame			= 0;
	obj._hitflag	= true;
	PlaySoundMem(breakSE, DX_PLAYTYPE_BACK);
	updater			= &DestroyableObject::BreakUpdate;
}

ObjectInfo DestroyableObject::GetInfo()
{
	return obj;
}
