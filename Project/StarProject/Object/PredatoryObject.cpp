#include "PredatoryObject.h"
#include "../Camera.h"


PredatoryObject::PredatoryObject(std::shared_ptr<Camera>& camera):Obstacle(camera),_camera(camera)
{
}


PredatoryObject::~PredatoryObject()
{
}
