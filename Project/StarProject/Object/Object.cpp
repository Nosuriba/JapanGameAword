#include "Object.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0,0,0);
	handle	 = 0;
	dieFlag  = hitFlag = false;
}

Object::~Object()
{
}

void Object::Init(std::string fileName, const Vector3& pos)
{
	this->pos	   = pos;
	this->fileName = fileName;
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());						/// モデルの読み込み

	/// 仮のカメラ設定
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0,20,-20), VGet(0.f, 10.f, 0.f));
}

void Object::Update()
{
}

void Object::Draw()
{
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// モデル座標の設定
	DxLib::MV1DrawModel(handle);
}
