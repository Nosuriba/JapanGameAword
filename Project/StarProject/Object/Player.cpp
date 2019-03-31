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
	/// �n���𓊂��鏈���Œǉ������Ƃ���B
	auto velControl = [](const Vector3& vel)
	{
		auto v = vel;
		v.x = (abs(v.x) < 0.1f ? v.x = 0 : v.x = v.x * 0.85f);
		v.y = (abs(v.y) < 0.1f ? v.y = 0 : v.y = v.y * 0.85f);

		return v;

	};
	vel	   = velControl(vel);
	pos   += vel;
	angle += Vector3(0.f, 0.01f, 0.f);			/// �n�������]�����Ă邾��


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
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));			/// ���f�����W�̐ݒ�
	DxLib::MV1SetRotationXYZ(handle, VGet(angle.x, angle.y, angle.z));  /// ���f���̉�]
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Player::DebugDraw()
{
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0, 0xffffff, "�n���̍��W(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "�n���̍��W(Y)  %d", (int)(pos.y));
	if (mouseDebug)
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "�N���b�N��", 0x0000ff);
	}
	else
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "�N���b�N���ĂȂ���", 0x0000ff);
	}
	
}

void Player::CheckHitModel(const Input & p)
{
	if (p.IsMousePressed(MOUSE_INPUT_LEFT))
	{
		VECTOR scrPos[2], worldPos[2];

		scrPos[0] = VGet((float)mouseX, (float)mouseY, 0);			// �X�N���[���̎n�_
		scrPos[1] = VGet((float)mouseX, (float)mouseY, 1.f);		// �X�N���[���̏I�_

		worldPos[0] = VGet(scrPos[0].x, scrPos[0].y, scrPos[0].z);	// ���[���h���W�̎n�_
		worldPos[1] = VGet(scrPos[1].x, scrPos[1].y, scrPos[1].z);	// ���[���h���W�̏I�_

		/// ���C�̎n�_�ƏI�_�̐ݒ���s���Ă���
		startPos = ConvScreenPosToWorldPos(worldPos[0]);
		endPos = ConvScreenPosToWorldPos(worldPos[1]);

		/// �ǂ̃��f���Ɠ����蔻�����邩�̐ݒ�
		DxLib::MV1RefreshCollInfo(handle, -1);

		/// ���C�����f���ɓ����������̔���
		auto rayHitCheck = DxLib::MV1CollCheck_Line(handle, -1, startPos, endPos);

		if (rayHitCheck.HitFlag)
		{
			catchFlag = true;
			catchPos = Vector3(mouseX, mouseY, 0);

			/// ���C�������������f���̍��W���擾���Ă���
			modelPos3D = DxLib::MV1GetPosition(handle);

			/// ���C�������������f���̈ʒu���擾���Ă���
			hitPos3D = rayHitCheck.HitPosition;
			hitPos2D = ConvWorldPosToScreenPos(hitPos3D);
			return;
		}
	}
}

void Player::MoveModel(const Input & p)
{
	///�@�}�C�i�X�ȉ��̒l���ł��؂��Ă�̂ŁA������T��
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

	/// ���f���̈ړ��������̍��W�����߂Ă���
	movePos = Vector3((float)(mouseX - catchPos.x),
					  (float)(mouseY - catchPos.y),
					   0);

	nowHitPos2D = VGet(hitPos2D.x + movePos.x,
					   hitPos2D.y + movePos.y,
					   hitPos2D.z);

	nowHitPos3D = ConvScreenPosToWorldPos(nowHitPos2D);

	/// �ړ���̃��f�����W��ݒ肵�Ă���B
	nowModelPos3D = VGet(modelPos3D.x + nowHitPos3D.x - hitPos3D.x,
						 modelPos3D.y + nowHitPos3D.y - hitPos3D.y,
						 modelPos3D.z + nowHitPos3D.z - hitPos3D.z);

	/// �n���𓊂��鏈���Œǉ������Ƃ���B
	vel = Vector3(Clamp(nowModelPos3D.x - pos.x), Clamp(nowModelPos3D.y - pos.y), 0);
	//pos = Vector3(nowModelPos3D.x, nowModelPos3D.y, 0);
}