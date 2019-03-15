#include "Object.h"

Object::Object()
{
	pos		= Vector3(0,0,0);
	dieFlag = hitFlag = false;
}

Object::~Object()
{
}

void Object::Init(std::string fileName, const Vector3& pos, int handle)
{
	this->pos = pos;
	DxLib::MV1LoadModel(fileName.c_str());						/// モデルの読み込み
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// モデル座標の設定(ここで必要ないなら消す)
}

void Object::Update()
{
}
