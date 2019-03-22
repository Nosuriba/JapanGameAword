#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{
	DxLib::GetMousePoint(&mouseX, &mouseY);

	if (catchFlag)
	{
		MoveModel();
	}
	else
	{
		CheckHitModel();
	}
}

void Player::Draw()
{
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// ���f�����W�̐ݒ�
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Player::DebugDraw()
{
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0, 0xffffff, "�n���̍��W(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "�n���̍��W(Y)  %d", (int)(pos.y));
}

void Player::CheckHitModel()
{
	if (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		VECTOR scrPos[2], worldPos[2];

		scrPos[0] = VGet((float)mouseX, (float)mouseY, 0);
		scrPos[1] = VGet((float)mouseX, (float)mouseY, 1.f);

		worldPos[0] = VGet(scrPos[0].x, scrPos[0].y, scrPos[0].z);
		worldPos[1] = VGet(scrPos[1].x, scrPos[1].y, scrPos[1].z);

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

void Player::MoveModel()
{
	if (!(DxLib::GetMouseInput() & MOUSE_INPUT_LEFT))
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

	pos = Vector3(nowModelPos3D.x, nowModelPos3D.y, 0);
}