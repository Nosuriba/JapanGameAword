#include "DestroyableObject.h"

#include "../ResourceManager.h"

const int offset = 60;

void DestroyableObject::NormalUpdate()
{
}

void DestroyableObject::BreakUpdate()
{
	//auto camera = _camera->CameraCorrection();
	//if (flame < 30) {
	//	Draw();
	//}
	//else 
	if (flame >= 30) {
		obj._breakflag = true;
		updater = &DestroyableObject::NormalUpdate;
	}
	flame++;
}

DestroyableObject::DestroyableObject(const std::shared_ptr<Camera>& camera, int x, int y, int level):Obstacle(camera)
{
	auto pos = Position2(x,y);
	auto size = Size(32 + offset,32 + offset);
	auto rect = Rect(pos, size);
	auto _level = level;

	obj = ObjectInfo(pos, size, rect, _level, false ,false);

	flame = 0;

	//LoadDivGraph("../img/destroy.png", 4, 2, 2, 624 / 2, 624 / 2, destroyactimg);
	//LoadDivGraph("../img/destroylevel2.png", 4, 2, 2, 624 / 2, 624 / 2, destroylevel2actimg);

	_img1 = ResourceManager::GetInstance().LoadImg("../img/destroy.png");
	_img2 = ResourceManager::GetInstance().LoadImg("../img/destroylevel2.png");

	updater = &DestroyableObject::NormalUpdate;
}

DestroyableObject::~DestroyableObject()
{
	//for (auto& gh : destroyactimg) {
	//	DeleteGraph(gh);
	//}
	//for (auto& gh : destroylevel2actimg) {
	//	DeleteGraph(gh);
	//}
}

void DestroyableObject::Draw()
{
	auto camera = _camera->CameraCorrection();
	
	if (obj._level == 1) {
		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() - camera.x,
		//	obj._rect.Top() - camera.y - (flame),
		//	obj._rect.Right() - obj._rect.size.width / 2 - camera.x,
		//	obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y - (flame),
		//	destroyactimg[0], true);

		// 左上
		DrawRectExtendGraph(
			obj._rect.Left()	- camera.x,
			obj._rect.Top()		- camera.y - (flame),
			obj._rect.Right()	- obj._rect.size.width / 2	- camera.x,
			obj._rect.Bottom()	- obj._rect.size.height / 2 - camera.y - (flame),
			0, 0, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() + obj._rect.size.width / 2 - camera.x + (flame),
		//	obj._rect.Top() - camera.y,
		//	obj._rect.Right() - camera.x + (flame),
		//	obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y,
		//	destroyactimg[1], true);

		// 右上
		DrawRectExtendGraph(
			obj._rect.Left()	+ obj._rect.size.width / 2	- camera.x + (flame),
			obj._rect.Top()		- camera.y,
			obj._rect.Right()	- camera.x + (flame),
			obj._rect.Bottom()	- obj._rect.size.height / 2 - camera.y,
			312, 0, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() - camera.x - (flame),
		//	obj._rect.Top() - camera.y + obj._rect.size.height / 2,
		//	obj._rect.Right() - obj._rect.size.height / 2 - camera.x - (flame),
		//	obj._rect.Bottom() - camera.y,
		//	destroyactimg[2], true);

		// 左下
		DrawRectExtendGraph(
			obj._rect.Left()	- camera.x - (flame),
			obj._rect.Top()		- camera.y + obj._rect.size.height / 2,
			obj._rect.Right()	- obj._rect.size.height / 2 - camera.x - (flame),
			obj._rect.Bottom()	- camera.y,
			0, 312, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() + obj._rect.size.width / 2 - camera.x,
		//	obj._rect.Top() + obj._rect.size.height / 2 - camera.y + (flame),
		//	obj._rect.Right() - camera.x,
		//	obj._rect.Bottom() - camera.y + (flame),
		//	destroyactimg[3], true);

		// 右下
		DrawRectExtendGraph(
			obj._rect.Left() + obj._rect.size.width / 2 - camera.x,
			obj._rect.Top() + obj._rect.size.height / 2 - camera.y + (flame),
			obj._rect.Right() - camera.x,
			obj._rect.Bottom() - camera.y + (flame),
			312, 312, 312, 312, _img1, true
		);
	}
	else if (obj._level == 2) {
		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() - camera.x,
		//	obj._rect.Top() - camera.y - (flame),
		//	obj._rect.Right() - obj._rect.size.width / 2 - camera.x,
		//	obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y - (flame),
		//	destroylevel2actimg[0], true);

		// 左上
		DrawRectExtendGraph(
			obj._rect.Left() - camera.x,
			obj._rect.Top() - camera.y - (flame),
			obj._rect.Right() - obj._rect.size.width / 2 - camera.x,
			obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y - (flame),
			0, 0, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() + obj._rect.size.width / 2 - camera.x + (flame),
		//	obj._rect.Top() - camera.y,
		//	obj._rect.Right() - camera.x + (flame),
		//	obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y,
		//	destroylevel2actimg[1], true);

		// 右上
		DrawRectExtendGraph(
			obj._rect.Left() + obj._rect.size.width / 2 - camera.x + (flame),
			obj._rect.Top() - camera.y,
			obj._rect.Right() - camera.x + (flame),
			obj._rect.Bottom() - obj._rect.size.height / 2 - camera.y,
			312, 0, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() - camera.x - (flame),
		//	obj._rect.Top() - camera.y + obj._rect.size.height / 2,
		//	obj._rect.Right() - obj._rect.size.height / 2 - camera.x - (flame),
		//	obj._rect.Bottom() - camera.y,
		//	destroylevel2actimg[2], true);

		// 左下
		DrawRectExtendGraph(
			obj._rect.Left() - camera.x - (flame),
			obj._rect.Top() - camera.y + obj._rect.size.height / 2,
			obj._rect.Right() - obj._rect.size.height / 2 - camera.x - (flame),
			obj._rect.Bottom() - camera.y,
			0, 312, 312, 312, _img1, true
		);

		//DxLib::DrawExtendGraph(
		//	obj._rect.Left() + obj._rect.size.width / 2 - camera.x,
		//	obj._rect.Top() + obj._rect.size.height / 2 - camera.y + (flame),
		//	obj._rect.Right() - camera.x,
		//	obj._rect.Bottom() - camera.y + (flame),
		//	destroylevel2actimg[3], true);

		// 右下
		DrawRectExtendGraph(
			obj._rect.Left() + obj._rect.size.width / 2 - camera.x,
			obj._rect.Top() + obj._rect.size.height / 2 - camera.y + (flame),
			obj._rect.Right() - camera.x,
			obj._rect.Bottom() - camera.y + (flame),
			312, 312, 312, 312, _img1, true
		);
	}
}

void DestroyableObject::Update()
{
	(this->*updater)();
}

void DestroyableObject::Break()
{
	flame = 0;
	obj._hitflag = true;
	updater = &DestroyableObject::BreakUpdate;
}

ObjectInfo DestroyableObject::GetInfo()
{
	return obj;
}
