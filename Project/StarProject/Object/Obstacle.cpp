#include "Obstacle.h"
#include "../Camera.h"

Obstacle::Obstacle(std::shared_ptr<Camera>& camera):_camera(camera)
{
	auto pos = Vector2();
	auto size = Size();
	auto rect = Rect(pos, size);
	auto obj = ObjectInfo(pos, size, rect);

	rockimg = LoadGraph("../img/rock.png");
}

Obstacle::~Obstacle()
{
}

void Obstacle::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (auto &obj : _obj) {
		DxLib::DrawExtendGraph(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
			obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, rockimg, true);

		DxLib::DrawBox(obj._rect.Left() - camera.x, obj._rect.Top() - camera.y,
			obj._rect.Right() - camera.x, obj._rect.Bottom() - camera.y, 0xff00ff, false);
	}
}

void Obstacle::Update()
{
}

void Obstacle::ObjCreate(const Vector2 & _pos, const Size & _size)
{
	auto pos = _pos;
	auto size = _size;
	auto rect = Rect(pos, size);
	auto obj = ObjectInfo(pos, size, rect);
	_obj.emplace_back(obj);
}

std::vector<ObjectInfo> Obstacle::GetObjectInfo()
{
	return _obj;
}
