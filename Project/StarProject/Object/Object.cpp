#include "Object.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0, 0, 0);
	scrPos	 = Vector3(0, 0, 0);
	startPos = VGet(0, 0, 0);
	endPos	 = VGet(0, 0, 0);
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
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());		/// ���f���̓ǂݍ���

	/// ���̃J�����ݒ�
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 20, -20), VGet(0.f, 10.f, 0.f));
}

void Object::Update()
{
	/// ���f���̓���m�F(���ݒ�)
	int mouseX, mouseY = 0;

	DxLib::GetMousePoint(&mouseX, &mouseY);
	scrPos = Vector3(mouseX, mouseY, 0);

	auto mouseScr = VGet(scrPos.x, scrPos.y, scrPos.z);
	startPos = ConvScreenPosToWorldPos(&mouseScr);


}

void Object::Draw()
{
	/// ���N���X
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// ���f�����W�̐ݒ�
	DxLib::MV1DrawModel(handle);
}