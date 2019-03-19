#include "Object.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0, 0, 0);
	scrPos	 = VGet(0, 0, 0);
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
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());		/// モデルの読み込み

	/// 仮のカメラ設定
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 20, -20), VGet(0.f, 10.f, 0.f));
}

void Object::Update()
{
	/// モデルの動作確認(仮設定)
	if (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		int mouseX, mouseY = 0;

		DxLib::GetMousePoint(&mouseX, &mouseY);
		scrPos = VGet(mouseX, mouseY, 0);

		auto mouseScr = VGet(scrPos.x, scrPos.y, scrPos.z);
		startPos = ConvScreenPosToWorldPos(mouseScr);

		scrPos.z = 1.f;
		endPos = ConvScreenPosToWorldPos(mouseScr);
		
		dbgRay = Vector3(endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z);
		
	}
	
}

void Object::Draw()
{
	/// 基底クラス
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// モデル座標の設定
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Object::DebugDraw()
{
	/// 座標がうまく渡せていないので、値が0のままになっている
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0, 0xffffff, "地球の座標(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "地球の座標(Y)  %d", (int)(pos.y));
	DxLib::DrawFormatString(0, dbg * 2, 0xffffff, "マウスの座標(X)  %d", (int)(scrPos.x));
	DxLib::DrawFormatString(0, dbg * 3, 0xffffff, "マウスの座標(Y)  %d", (int)(scrPos.y));
}
