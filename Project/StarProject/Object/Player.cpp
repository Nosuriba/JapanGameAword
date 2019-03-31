#include "Player.h"
#include "../Input.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(const Input & p)
{
	/// 地球を投げる処理で追加したところ。
	auto velControl = [](const Vector3& vel)
	{
		auto v = vel;
		v.x = (abs(v.x) < 0.1f ? v.x = 0 : v.x = v.x * 0.85f);
		v.y = (abs(v.y) < 0.1f ? v.y = 0 : v.y = v.y * 0.85f);

		return v;

	};
	vel	   = velControl(vel);
	pos   += vel;
	angle += Vector3(0.f, 0.01f, 0.f);			/// 地球を自転させてるだけ


	DxLib::GetMousePoint(&mouseX, &mouseY);

	if (catchFlag)
	{
		MoveModel(p);
	}
	else
	{
		CheckHitModel(p);
	}

	mouseDebug = p.IsMousePressed(MOUSE_INPUT_LEFT);
}

void Player::Draw()
{
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));			/// モデル座標の設定
	DxLib::MV1SetRotationXYZ(handle, VGet(angle.x, angle.y, angle.z));  /// モデルの回転
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Player::DebugDraw()
{
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0, 0xffffff, "地球の座標(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "地球の座標(Y)  %d", (int)(pos.y));
	if (mouseDebug)
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "クリック中", 0x0000ff);
	}
	else
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "クリックしてないよ", 0x0000ff);
	}
	
}

void Player::CheckHitModel(const Input & p)
{
	if (p.IsMousePressed(MOUSE_INPUT_LEFT))
	{
		VECTOR scrPos[2], worldPos[2];

		scrPos[0] = VGet((float)mouseX, (float)mouseY, 0);			// スクリーンの始点
		scrPos[1] = VGet((float)mouseX, (float)mouseY, 1.f);		// スクリーンの終点

		worldPos[0] = VGet(scrPos[0].x, scrPos[0].y, scrPos[0].z);	// ワールド座標の始点
		worldPos[1] = VGet(scrPos[1].x, scrPos[1].y, scrPos[1].z);	// ワールド座標の終点

		/// レイの始点と終点の設定を行っている
		startPos = ConvScreenPosToWorldPos(worldPos[0]);
		endPos = ConvScreenPosToWorldPos(worldPos[1]);

		/// どのモデルと当たり判定を取るかの設定
		DxLib::MV1RefreshCollInfo(handle, -1);

		/// レイがモデルに当たったかの判定
		auto rayHitCheck = DxLib::MV1CollCheck_Line(handle, -1, startPos, endPos);

		if (rayHitCheck.HitFlag)
		{
			catchFlag = true;
			catchPos = Vector3(mouseX, mouseY, 0);

			/// レイが当たったモデルの座標を取得している
			modelPos3D = DxLib::MV1GetPosition(handle);

			/// レイが当たったモデルの位置を取得している
			hitPos3D = rayHitCheck.HitPosition;
			hitPos2D = ConvWorldPosToScreenPos(hitPos3D);
			return;
		}
	}
}

void Player::MoveModel(const Input & p)
{
	///　マイナス以下の値が打ち切られてるので、原因を探る
	auto Clamp = [](const float& val, const float& minVal = -10.0f, const float& maxVal = 10.0f)
	{
		return  max(minVal, min(maxVal, val));
	};

	if (!(p.IsMousePressed(MOUSE_INPUT_LEFT)))
	{
		catchFlag = false;
	}

	Vector3 movePos;
	VECTOR nowHitPos2D;
	VECTOR nowHitPos3D;
	VECTOR nowModelPos3D;

	/// モデルの移動した分の座標を求めている
	movePos = Vector3((float)(mouseX - catchPos.x),
					  (float)(mouseY - catchPos.y),
					   0);

	nowHitPos2D = VGet(hitPos2D.x + movePos.x,
					   hitPos2D.y + movePos.y,
					   hitPos2D.z);

	nowHitPos3D = ConvScreenPosToWorldPos(nowHitPos2D);

	/// 移動後のモデル座標を設定している。
	nowModelPos3D = VGet(modelPos3D.x + nowHitPos3D.x - hitPos3D.x,
						 modelPos3D.y + nowHitPos3D.y - hitPos3D.y,
						 modelPos3D.z + nowHitPos3D.z - hitPos3D.z);

	/// 地球を投げる処理で追加したところ。
	vel = Vector3(Clamp(nowModelPos3D.x - pos.x), Clamp(nowModelPos3D.y - pos.y), 0);
	//pos = Vector3(nowModelPos3D.x, nowModelPos3D.y, 0);
}