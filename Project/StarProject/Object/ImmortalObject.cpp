#include "ImmortalObject.h"
#include "../Camera.h"


ImmortalObject::ImmortalObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
}


ImmortalObject::~ImmortalObject()
{
}
