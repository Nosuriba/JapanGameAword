#include "GoalObject.h"
#include "../ResourceManager.h"


GoalObject::GoalObject(const std::shared_ptr<Camera>& camera, int x, int y):Obstacle(camera)
{
}


GoalObject::~GoalObject()
{
}
