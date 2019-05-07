#include "DestroyableObject.h"
#include "../Camera.h"


DestroyableObject::DestroyableObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
}


DestroyableObject::~DestroyableObject()
{
}
