#include "GoalObject.h"
#include "../ResourceManager.h"

constexpr int offset(60);

GoalObject::GoalObject(const std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera)
{
	auto pos = Position2(x, y);
	auto size = Size(32 + offset, 32 + offset);
	auto rect = Rect(pos, size);

	obj = ObjectInfo(pos, size, rect, 0, false, false);
	goalimg = ResourceManager::GetInstance().LoadImg("Goal.png");
}

GoalObject::~GoalObject()
{
}

void GoalObject::Draw()
{
	auto camera = _camera->CameraCorrection();

	DrawExtendGraph(
		obj._rect.Left() - camera.x,
		obj._rect.Top() - camera.y,
		obj._rect.Right() - camera.x,
		obj._rect.Bottom() - camera.y,
		goalimg, true);
}
