#include "Object.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0, 0, 0);
	catchPos = Vector3(0, 0, 0);
	startPos = VGet(0, 0, 0);
	endPos	 = VGet(0, 0, 0);
	hitPos2D = VGet(0, 0, 0);
	hitPos3D = VGet(0, 0, 0);
	modelPos3D = VGet(0, 0, 0);
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
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());		/// モデルの読み込み

	/// 仮のカメラ設定
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 20, -20), VGet(0.f, 10.f, 0.f));
}

void Object::Update()
{
	int mouseX, mouseY = 0;

	DxLib::GetMousePoint(&mouseX, &mouseY);
	
	/// モデルの動作確認(仮設定)
	if (!catchFlag)
	{
		if (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			/// endとstartの値が同じになっているので、その原因を探る
			VECTOR scrPos, scrPos2, worldPos, worldPos2;

			scrPos = VGet((float)mouseX, (float)mouseY, 0);
			scrPos2 = VGet((float)mouseX, (float)mouseY, 1.f);

			worldPos = VGet(scrPos.x, scrPos.y, scrPos.z);
			worldPos2 = VGet(scrPos2.x, scrPos2.y, scrPos2.z);

			startPos = ConvScreenPosToWorldPos(worldPos);
			endPos = ConvScreenPosToWorldPos(worldPos2);

			DxLib::MV1RefreshCollInfo(handle, -1);

			auto result = DxLib::MV1CollCheck_Line(handle, -1, startPos, endPos);

			if (result.HitFlag)
			{
				catchFlag = true;

				catchPos = Vector3(mouseX, mouseY, 0);

				modelPos3D = DxLib::MV1GetPosition(handle);

				hitPos3D = result.HitPosition;
				hitPos2D = ConvWorldPosToScreenPos(hitPos3D);

			}
		}
	}
	else
	{
		if (!(DxLib::GetMouseInput() & MOUSE_INPUT_LEFT))
		{
			catchFlag = false;
		}

		float moveX, moveY;
		VECTOR nowHitPos2D;
		VECTOR nowHitPos3D;
		VECTOR nowModelPos3D;

		moveX = (float)(mouseX - catchPos.x);
		moveY = (float)(mouseY - catchPos.y);

		nowHitPos2D = VGet(hitPos2D.x + moveX,
						   hitPos2D.y + moveY,
						   hitPos2D.z);

		nowHitPos3D = ConvScreenPosToWorldPos(nowHitPos2D);

		nowModelPos3D = VGet(modelPos3D.x + nowHitPos3D.x,
							 modelPos3D.y + nowHitPos3D.y,
							 modelPos3D.z + nowHitPos3D.z);

		DxLib::MV1SetPosition(handle, nowModelPos3D);
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
}
