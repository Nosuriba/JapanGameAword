#include "Object.h"
#include "../Input.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0, 0, 0);
	vel		 = Vector3(0, 0, 0);
	angle	 = Vector3(0, 0, 0);
	catchPos = Vector3(0, 0, 0);

	startPos = endPos = hitPos2D = hitPos3D = modelPos3D = VGet(0, 0, 0);

	mouseX	 = mouseY = 0;
	handle	 = 0;
	dieFlag  = catchFlag = hitFlag = false;
}

Object::~Object()
{
}

void Object::Init(std::string fileName, const Vector3& pos)
{
	this->pos	   = pos;
	this->fileName = fileName;
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());		/// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ

	/// ‰¼‚ÌƒJƒƒ‰İ’è
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 20, -20), VGet(0.f, 10.f, 0.f));
}

void Object::Update(const Input & p)
{
}

void Object::Draw()
{
}
